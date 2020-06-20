//
// Created by ldr on 2020/6/19.
//

#include "custom_filter.h"

short *CustomFilter::input_data = nullptr;
int CustomFilter::input_size = 0;
short *CustomFilter::out_data = nullptr;

static int input_drain(sox_effect_t *effp, sox_sample_t *obuf, size_t *osamp) {
    *osamp = static_cast<size_t>(CustomFilter::input_size);
    int i = 0;
    for (; i < *osamp; i++) {
        obuf[i] = ((sox_sample_t) (CustomFilter::input_data[i]) << 16);
    }
    return SOX_SUCCESS;
}

static int output_flow(sox_effect_t *effp LSX_UNUSED, sox_sample_t const *ibuf, sox_sample_t *obuf LSX_UNUSED, size_t *isamp, size_t *osamp) {
    if (*isamp) {
        int i = 0;
        for (; i < *isamp; i++) {
            CustomFilter::out_data[i] = (short) (ibuf[i] >> 16);
        }
    }
    *osamp = 0;
    return *isamp == 0 ? SOX_SUCCESS : SOX_EOF;
}

static sox_effect_handler_t const *input_handler() {
    static sox_effect_handler_t handler = {"input", nullptr, SOX_EFF_MCHAN | SOX_EFF_MODIFY, nullptr, nullptr, nullptr, input_drain, nullptr, nullptr, 0};
    return &handler;
}

static sox_effect_handler_t const *output_handler() {
    static sox_effect_handler_t handler = {"output", nullptr, SOX_EFF_MCHAN | SOX_EFF_MODIFY | SOX_EFF_PREC, nullptr, nullptr, output_flow, nullptr, nullptr, nullptr, 0};
    return &handler;
}

void CustomFilter::init(int sampleRate, int channelCount) {
    this->sample_rate = sampleRate;
    this->channels = channelCount;

    sox_globals.bufsiz = static_cast<size_t>(2048);

    encoding.encoding = (sox_encoding_t) 1;
    encoding.bits_per_sample = 16;
    encoding.reverse_bytes = (sox_option_t) 0;
    encoding.reverse_nibbles = (sox_option_t) 0;
    encoding.reverse_bits = (sox_option_t) 0;
    encoding.compression = INFINITY;
    encoding.opposite_endian = (sox_bool) 0;

    signal.precision = 16;
    signal.channels = static_cast<unsigned int>(channelCount);
    signal.rate = sampleRate;
    signal.length = (sox_uint64_t) sampleRate * channelCount * 15 * 60;
    signal.mult = nullptr;

    mono_signal = signal;
    mono_signal.channels = 1;

    float arr[] = {0.5f, 0.96f, 0.5f, 0.76f, 0.08f, 0.62f, 0.33f, 0.92f};
    initChain(arr);

    out_data = new short[2048];
}

void CustomFilter::initChain(const float *arr) {
    float reverbRatio = arr[0];
    float reverbDeep = arr[1];
    float reverbGain = arr[2];

    float minDelay = arr[3];
    float maxDelay = arr[4];

    float compressLimit = arr[5];
    float compressRange = arr[6];
    float compressGain = arr[7];

    dryWetMix = static_cast<float>((tan((reverbGain - 0.44) * M_PI / 1.5) + 1.4) / 4);


    int input1 = -75;
    int output1 = -100;
    int input3 = (compressLimit * 30) - 40;
    int output3 = input3;
    int input2 = input3 - 20;
    int output2 = input3 - 20;

    int t = -5 - input3;
    t = t < 20 ? t : 20;
    t = t > 5 ? t : 5;
    t -= 0.5;
    int input4 = 0;
    int output4 = compressRange * t + output3;

    t = -1 - output4;
    t = t > 0 ? t : 0;
    t = t < 40 ? t : 40;
    t *= 0.75;
    int cmp_gain = t + compressGain * 20 - 20;;


    float attackTime = 0.02f;
    float decayTime = 0.04f;

    int reverbeRance = 40 + reverbRatio * 60;
    int roomScale = 40 + reverbRatio * 60;
    int preDelay = 56 + reverbDeep * 27;
    int damping = reverbDeep * 10;
    int wetGain = 3;
    int stereoDepth = 83;


    effect_chain = sox_create_effects_chain(&encoding, &encoding);

    sox_effect_t *e = sox_create_effect(input_handler());
    assert(sox_add_effect(effect_chain, e, &signal, &signal) == SOX_SUCCESS);
    free(e);

    if (channels == 2) {
        e = sox_create_effect(sox_find_effect("channels"));
        assert(sox_effect_options(e, 0, nullptr) == SOX_SUCCESS);
        assert(sox_add_effect(effect_chain, e, &signal, &mono_signal) == SOX_SUCCESS);
        free(e);
    }

    int compaundArgs = 5;
    char *attackRelease = new char[20];
    sprintf(attackRelease, "%g,%g", attackTime, decayTime);
    char *functionTransforTable = new char[100];
    sprintf(functionTransforTable, "%d:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 3, -100, input1, output1, input2, output2, input3, output3, input4, output4, 0, 0, 0, 0);
    char *gain = new char[10];
    sprintf(gain, "%d", cmp_gain);
    char *initialVolume = new char[10];
    sprintf(initialVolume, "%d", -100);
    char *delay = new char[10];
    sprintf(delay, "%g", 0.0);
    char *compaundArgv[] = {attackRelease, functionTransforTable, gain, initialVolume, delay};
    e = sox_create_effect(sox_find_effect("compand"));
    assert(sox_effect_options(e, compaundArgs, compaundArgv) == SOX_SUCCESS);
    assert(sox_add_effect(effect_chain, e, &mono_signal, &mono_signal) == SOX_SUCCESS);
    for (int i = 0; i < compaundArgs; i++) {
        delete[] compaundArgv[i];
    }
    free(e);

    if (channels == 2) {
        e = sox_create_effect(sox_find_effect("channels"));
        assert(sox_effect_options(e, 0, nullptr) == SOX_SUCCESS);
        assert(sox_add_effect(effect_chain, e, &mono_signal, &signal) == SOX_SUCCESS);
        free(e);
    }

    int equalizerArgs = 3;
    char *frequency = new char[10];
    sprintf(frequency, "%d", 5800 + (int) (2800 * minDelay));
    char *width = new char[10];
    sprintf(width, "%0.2fq", 0.2 + 1 - maxDelay);
    char *eq_gain = new char[10];
    sprintf(eq_gain, "%d", 1);
    char *equalizerArgv[] = {frequency, width, eq_gain};
    e = sox_create_effect(sox_find_effect("equalizer"));
    assert(sox_effect_options(e, equalizerArgs, equalizerArgv) == SOX_SUCCESS);
    for (int i = 0; i < equalizerArgs; i++) {
        delete[] equalizerArgv[i];
    }

    int reverbArgs = 7;
    char *e_wetOnly = new char[10];
    sprintf(e_wetOnly, "%s", "-w");
    char *e_reverbrance = new char[10];
    sprintf(e_reverbrance, "%d", reverbeRance);
    char *e_damping = new char[10];
    sprintf(e_damping, "%d", damping);
    char *e_roomScale = new char[10];
    sprintf(e_roomScale, "%d", roomScale);
    char *e_stereoDepth = new char[10];
    sprintf(e_stereoDepth, "%d", stereoDepth);
    char *e_preDelay = new char[10];
    sprintf(e_preDelay, "%d", preDelay);
    char *e_wetGain = new char[10];
    sprintf(e_wetGain, "%d", wetGain);


    char *reverbArgv[] = {e_wetOnly, e_reverbrance, e_damping, e_roomScale, e_stereoDepth, e_preDelay, e_wetGain};

    e = sox_create_effect(sox_find_effect("reverb"));
    assert(sox_effect_options(e, reverbArgs, reverbArgv) == SOX_SUCCESS);
    assert(sox_add_effect(effect_chain, e, &signal, &signal) == SOX_SUCCESS);
    free(e);
    for (int i = 0; i < reverbArgs; i++) {
        delete[] reverbArgv[i];
    }

    e = sox_create_effect(output_handler());
    assert(sox_add_effect(effect_chain, e, &signal, &signal) == SOX_SUCCESS);
    free(e);
    inited = true;
}

void CustomFilter::destroyChain() {
    if (nullptr != effect_chain && inited) {
        sox_delete_effects_chain(effect_chain);
        effect_chain = nullptr;
    }
    inited = false;
}

void CustomFilter::setEffect(float *arr) {
    destroyChain();
    initChain(arr);
}

int CustomFilter::process(short *data, int len) {
    input_size = len;
    input_data = data;
    auto *directAudioBuffer = new short[len];
    memcpy(directAudioBuffer, data, len * sizeof(short));
    if (effect_chain != nullptr) {
        sox_flow_effects(effect_chain, nullptr, nullptr);
        memcpy(data, out_data, len * sizeof(short));
        for (int i = 0; i < len; i++) {
            data[i] = static_cast<short>(directAudioBuffer[i] * (1 - dryWetMix) + out_data[i] * dryWetMix);
        }
    }
    delete[] directAudioBuffer;
    return len;
}

void CustomFilter::destroy() {
    destroyChain();
    DELETEARR(out_data)
}