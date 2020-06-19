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

static int compressor_output_flow(sox_effect_t *effp LSX_UNUSED, sox_sample_t const *ibuf, sox_sample_t *obuf LSX_UNUSED, size_t *isamp, size_t *osamp) {
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
    static sox_effect_handler_t handler = {"output", nullptr, SOX_EFF_MCHAN | SOX_EFF_MODIFY | SOX_EFF_PREC, nullptr, nullptr, compressor_output_flow, nullptr, nullptr, nullptr, 0};
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

    float arr[] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};
    initChain(arr);
}

void CustomFilter::initChain(float *arr) {

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
    int gain = t + compressGain * 20 - 20;


    float attackTime = 0.02f;
    float decayTime = 0.04f;

    int reverberance = 40 + reverbRatio * 60;
    int roomScale = 40 + reverbRatio * 60;

    int echoPredecay = 70 - reverbDeep * 60;
    int preDelay = 56 + reverbDeep * 27;
    int damping = reverbDeep * 10;

    int echoDecayRatio = 20 + minDelay * 60;

    int echoDelay = 10 + maxDelay * 120;
    int echoPeriod = 30 + maxDelay * 110;
    if (maxDelay > 0.9) {
        echoPeriod += 700 * (maxDelay - 0.9);
    }

    int wetGain = 3;
    int echoNum = 5;
    int echoPhaseDiff = 22;
    int stereoDepth = 83;


    effect_chain = sox_create_effects_chain(&encoding, &encoding);

    sox_effect_t *e = sox_create_effect(input_handler());
    assert(sox_add_effect(effect_chain, e, &signal, &signal) == SOX_SUCCESS);
    free(e);

    if (channels == 2) {
        e = sox_create_effect(sox_find_effect("channels"));
        assert(sox_effect_options(e, 0, NULL) == SOX_SUCCESS);
        assert(sox_add_effect(effect_chain, e, &signal, &mono_signal) == SOX_SUCCESS);
        free(e);
    }

    int compaundArgs = 5;
    char *attackRelease = new char[20];
    compressorFilterParam->getAttackRelease(&attackRelease);
    char *functionTransforTable = new char[100];
    compressorFilterParam->getFunctionTransforTable(&functionTransforTable);
    char *gain = new char[10];
    compressorFilterParam->getGain(&gain);
    char *initialVolume = new char[10];
    compressorFilterParam->getInitialVolume(&initialVolume);
    char *delay = new char[10];
    compressorFilterParam->getDelay(&delay);
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
    equalizerFilterParam->getFrequency(&frequency);
    char *width = new char[10];
    equalizerFilterParam->getWidth(&width);
    char *gain = new char[10];
    equalizerFilterParam->getGain(&gain);
    char *equalizerArgv[] = {frequency, width, gain};
    e = sox_create_effect(sox_find_effect("equalizer"));
    assert(sox_effect_options(e, equalizerArgs, equalizerArgv) == SOX_SUCCESS);
    for (int i = 0; i < equalizerArgs; i++) {
        delete[] equalizerArgv[i];
    }

    int reverbArgs = 7;
    char *wetOnly = new char[10];
    reverbFilterParam->getWetOnly(&wetOnly);
    char *reverbrance = new char[10];
    reverbFilterParam->getReverbrance(&reverbrance);
    char *damping = new char[10];
    reverbFilterParam->getDamping(&damping);
    char *roomScale = new char[10];
    reverbFilterParam->getRoomScale(&roomScale);
    char *stereoDepth = new char[10];
    reverbFilterParam->getStereoDepth(&stereoDepth);
    char *preDelay = new char[10];
    reverbFilterParam->getPreDelay(&preDelay);
    char *wetGain = new char[10];
    reverbFilterParam->getWetGain(&wetGain);


    char *reverbArgv[] = {wetOnly, reverbrance, damping, roomScale, stereoDepth, preDelay, wetGain};

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
    if (effect_chain != nullptr) {
        sox_flow_effects(effect_chain, nullptr, nullptr);
        memcpy(data, out_data, len * sizeof(short));
    }
    return len;
}

void CustomFilter::destroy() {
    destroyChain();
    DELETEARR(out_data)
    DELETEARR(input_data)
}