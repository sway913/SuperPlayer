#include "ff_filter.h"

#define INPUT_FORMAT         AV_SAMPLE_FMT_S16

void getEchoOptionString(char *str, size_t size, int type) {
    switch (type) {
        case EFFECT_PRESET_NONE:
            snprintf(str, size, "");
            break;
        case EFFECT_PRESET_CINEMA:
            snprintf(str, size, "0.8:0.9:60|120:0.3|0.25");
            break;
        case EFFECT_PRESET_KTV:
            snprintf(str, size, "0.8:0.9:200|201:0.3|0.25");
            break;
        case EFFECT_PRESET_STUDIO:
            snprintf(str, size, "0.8:0.9:15|60:0.3|0.25");
            break;
        case EFFECT_PRESET_CONCERT:
            snprintf(str, size, "0.8:0.7:60|120:0.15|0.1");
            break;
        case EFFECT_PRESET_CLASSIC_MAN:
            snprintf(str, size, "0.8:0.9:30|90:0.3|0.25");
            break;
        case EFFECT_PRESET_RECORD_PLAYER:
            snprintf(str, size, "0.9:0.9:30:0.3");
            break;
        case EFFECT_PRESET_CLASSIC_WOMAN:
            snprintf(str, size, "0.8:0.9:15|90:0.3|0.25");
            break;
        case EFFECT_PRESET_VALLEY:
            snprintf(str, size, "0.8:0.9:300|301:0.4|0.25");
            break;
        case EFFECT_PRESET_POP:
            snprintf(str, size, "0.99:0.4:94:0.59");
            break;
        case EFFECT_PRESET_RB:
            snprintf(str, size, "0.99:0.4:22:0.9");
            break;
        case EFFECT_PRESET_ROCK:
            snprintf(str, size, "0.99:0.4:47:0.5");
            break;
        case EFFECT_PRESET_HIPHOP:
            snprintf(str, size, "0.99:0.4:148:0.43");
            break;
        case EFFECT_PRESET_SPACIOUS:
            snprintf(str, size, "0.99:0.4:169:0.41");
            break;
        case EFFECT_PRESET_ORIGINAL:
            snprintf(str, size, "0.99:0.4:8:0.46");
            break;
        case EFFECT_PRESET_TEST:
            snprintf(str, size, "0.8:0.9:1000|1800:0.3|0.25");
            break;
        default:
            snprintf(str, size, "");
            break;
    }
};

void getEqualizerOptionString(char *str, size_t size, int type) {
    switch (type) {
        case EFFECT_PRESET_NONE:
            snprintf(str, size, "");
            break;
        case EFFECT_PRESET_CINEMA:
            snprintf(str, size,
                     "c0 f=250 w=275 g=3 t=1|c0 f=500 w=450 g=-3 t=1|c0 f=3000 w=2400 g=2 t=1|c0 f=7000 w=9800 g=3 t=1");
            break;
        case EFFECT_PRESET_KTV:
            snprintf(str, size,
                     "c0 f=300 w=360 g=2 t=1|c0 f=600 w=360 g=-2 t=1|c0 f=3000 w=2400 g=2 t=1|c0 f=7000 w=6300 g=2 t=1");
            break;
        case EFFECT_PRESET_STUDIO:
            snprintf(str, size, "");
            break;
        case EFFECT_PRESET_CONCERT:
            snprintf(str, size,
                     "c0 f=300 w=360 g=2 t=1|c0 f=500 w=450 g=-3 t=1|c0 f=3000 w=2400 g=2 t=1|c0 f=7000 w=6300 g=3 t=1");
            break;
        case EFFECT_PRESET_CLASSIC_MAN:
            snprintf(str, size,
                     "c0 f=250 w=200 g=4 t=1|c0 f=500 w=500 g=-4 t=1|c0 f=4000 w=4800 g=3 t=1|c0 f=10000 w=12000 g=2 t=1");
            break;
        case EFFECT_PRESET_RECORD_PLAYER:
            snprintf(str, size,
                     "c0 f=200 w=400 g=-12 t=1|c0 f=700 w=1260 g=6 t=1|c0 f=5000 w=9000 g=-7 t=1|c0 f=10000 w=18000 g=-12 t=1");
            break;
        case EFFECT_PRESET_CLASSIC_WOMAN:
            snprintf(str, size,
                     "c0 f=300 w=390 g=-4 t=1|c0 f=800 w=560 g=-2 t=1|c0 f=3000 w=1800 g=2 t=1|c0 f=8000 w=5600 g=3 t=1");
            break;
        case EFFECT_PRESET_VALLEY:
            snprintf(str, size,
                     "c0 f=200 w=200 g=2 t=1|c0 f=500 w=500 g=-2 t=1|c0 f=1000 w=1000 g=1 t=1|c0 f=7000 w=14000 g=2 t=1");
            break;
        case EFFECT_PRESET_POP:
            snprintf(str, size, "");
            break;
        case EFFECT_PRESET_RB:
            snprintf(str, size, "");
            break;
        case EFFECT_PRESET_ROCK:
            snprintf(str, size,
                     "c0 f=1200 w=1200 g=3 t=1|c0 f=100 w=300 g=2 t=1|c0 f=800 w=2000 g=-3 t=1|c0 f=3200 w=6400 g=6 t=1");
            break;
        case EFFECT_PRESET_HIPHOP:
            snprintf(str, size,
                     "c0 f=50 w=50 g=3 t=1|c0 f=100 w=100 g=6 t=1|c0 f=200 w=200 g=3.3 t=1|c0 f=400 w=400 g=2.5 t=1|c0 f=800 w=800 g=1.5 t=1|c0 f=1600 w=1600 g=0 t=1|c0 f=3150 w=3150 g=0 t=1|c0 f=6300 w=6300 g=2 t=1|c0 f=12500 w=12500 g=2 t=1");
            break;
        case EFFECT_PRESET_SPACIOUS:
            snprintf(str, size,
                     "c0 f=50 w=50 g=2 t=1|c0 f=100 w=100 g=3 t=1|c0 f=200 w=200 g=2 t=1|c0 f=400 w=400 g=1 t=1|c0 f=800 w=800 g=0 t=1|c0 f=2000 w=2000 g=0 t=1|c0 f=5000 w=5000 g=4 t=1|c0 f=8000 w=1600 g=-7 t=1|c0 f=12500 w=12500 g=2 t=1");
            break;
        case EFFECT_PRESET_ORIGINAL:
            snprintf(str, size, "");
            break;
        default:
            snprintf(str, size, "");
            break;
    }
}

void FFFilter::init(int sampleRate, int channelCount) {
    this->sample_rate = sampleRate;
    this->channels = channelCount;
    if (!init()) {
        LOGE("AudioFilter::AudioFilter init failed");
    }
}

bool FFFilter::init() {

    int err;
    avFrame_ = av_frame_alloc();

    memset(avFrame_, 0, sizeof(AVFrame));
    if (!avFrame_) {
        LOGI("Error allocating the frame\n");
        return false;
    }

    err = initFilterGraph(&filterGraph_, &avSrc_, &avSink_);
    if (err < 0) {
        LOGI("Unable to init filter graph:");
        return false;
    }

    LOGD(" --------- AudioFilter::init success ---------");

    return true;
}


int FFFilter::process(short *input, int len) {

    if (!avSrc_ || !avFrame_ || !avSink_) {
        return -1;
    }

    ++processCount;
    if (!input) {
        LOGE("%s null input", __FUNCTION__);
        return -1;
    }

    if (isConfiguring) {
        LOGW("AudioFilter::process isConfiguring true");
        return len;
    }

    std::lock_guard<std::mutex> lock(configureMutex_);

    uint64_t channelLayout;
    switch (channels) {
        case 1:
            channelLayout = AV_CH_LAYOUT_MONO;
            break;
        case 2:
            channelLayout = AV_CH_LAYOUT_STEREO;
            break;
        default:
            channelLayout = AV_CH_LAYOUT_MONO;
    }

    int format;
    switch (static_cast<uint16_t>(SL_PCMSAMPLEFORMAT_FIXED_16)) {
        case 8:
            format = AV_SAMPLE_FMT_U8;
            break;
        case 16:
            format = AV_SAMPLE_FMT_S16;
            break;
        case 32:
            format = AV_SAMPLE_FMT_S32;
            break;
        default:
            format = AV_SAMPLE_FMT_S16;
    }

    avFrame_->sample_rate = sample_rate;
    avFrame_->format = format;
    avFrame_->channel_layout = channelLayout;
    avFrame_->nb_samples = len / channels;
    avFrame_->pts = processCount * len / channels;


    int err = 0;
    err = av_frame_get_buffer(avFrame_, 0);
    if (err < 0) {
        LOGE("AudioFilter::init error");
        printError(err);
        return err;
    }
    memcpy(avFrame_->extended_data[0], input, len * sizeof(short));
    avFrame_->linesize[0] = len * sizeof(short);

    /* Send the frame to the input of the filtergraph. */
    err = av_buffersrc_add_frame(avSrc_, avFrame_);
    if (err < 0) {
        LOGE("AudioFilter::init error");
        av_frame_unref(avFrame_);
        fprintf(stderr, "Error submitting the frame to the filtergraph:");
        printError(err);
        LOGE("AudioFilter::process error 2.");
        return -1;
    }

    /* Get all the filtered output that is available. */
    err = av_buffersink_get_frame(avSink_, avFrame_);
    /* now do something with our filtered frame */
    if (err < 0) {
        av_frame_unref(avFrame_);

        fprintf(stderr, "Error processing the filtered frame:");
        LOGE("AudioFilter::process error 3");
        return err;
    }

    memcpy(input, avFrame_->extended_data[0], len * sizeof(short));
    av_frame_unref(avFrame_);

    return len;
}

void FFFilter::setVolume(double vol) {
    if (volume == vol) {
        return;
    }
    volume = vol;
    LOGD("audiofilter volume is %f ", vol);
    reconfigure();
}

void FFFilter::setFilter(int filter) {
    if (preset == filter) {
        return;
    }
    preset = filter;
    LOGD("audiofilter effect is %d ", filter);
    reconfigure();
}

bool FFFilter::reconfigure() {
    std::lock_guard<std::mutex> lock(configureMutex_);

    if (filterGraph_) {
        avfilter_graph_free(&filterGraph_);
        filterGraph_ = nullptr;
    }
    int err = initFilterGraph(&filterGraph_, &avSrc_, &avSink_);
    if (err < 0) {
        fprintf(stderr, "Unable to init filter graph:");
        printError(err);
        return false;
    }
    return true;
}

void FFFilter::destroy() {
    if (avSrc_) {
        avfilter_free(avSrc_);
        avSrc_ = nullptr;
    }
    if (avSink_) {
        avfilter_free(avSink_);
        avSink_ = nullptr;
    }
    if (filterGraph_) {
        avfilter_graph_free(&filterGraph_);
        filterGraph_ = nullptr;
    }
    if (avFrame_) {
        av_frame_free(&avFrame_);
        avFrame_ = nullptr;
    }
}

void FFFilter::printError(int err) {
    char errstr[1024];
    av_strerror(err, errstr, sizeof(errstr));
    LOGE("AudioFilter::printError description: %s", errstr);
}

int FFFilter::initFilterGraph(AVFilterGraph **graph, AVFilterContext **src, AVFilterContext **sink) {
    AVFilterGraph *filter_graph;

    AVFilterContext *abuffer_ctx;
    const AVFilter *abuffer;

    AVFilterContext *aecho_ctx;
    const AVFilter *aecho;

    AVFilterContext *volume_ctx;
    const AVFilter *avolume;

    AVFilterContext *aformat_ctx;
    const AVFilter *aformat;

    AVFilterContext *abuffersink_ctx;
    const AVFilter *abuffersink;

    AVFilterContext *anequalizer_ctx;
    const AVFilter *anequalizer;

    AVDictionary *options_dict = nullptr;
    char options_str[1024];
    char ch_layout[64];

    int err;
    isConfiguring = true;

    /* Create a new filtergraph, which will contain all the filters. */
    filter_graph = avfilter_graph_alloc();
    if (!filter_graph) {
        fprintf(stderr, "Unable to create filter graph.\n");
        LOGE("AudioFilter::initFilterGraph 1");
        isConfiguring = false;
        return AVERROR(ENOMEM);
    }

    /* Create the abuffer filter;
     * it will be used for feeding the data into the graph. */
    abuffer = avfilter_get_by_name("abuffer");
    if (!abuffer) {
        fprintf(stderr, "Could not find the abuffer filter.\n");
        LOGE("AudioFilter::initFilterGraph 2");
        isConfiguring = false;
        return AVERROR_FILTER_NOT_FOUND;
    }

    abuffer_ctx = avfilter_graph_alloc_filter(filter_graph, abuffer, "src");
    if (!abuffer_ctx) {
        fprintf(stderr, "Could not allocate the abuffer instance.\n");
        LOGE("AudioFilter::initFilterGraph 3");
        isConfiguring = false;
        return AVERROR(ENOMEM);
    }

    /* Set the filter options through the AVOptions API. */
    switch (channels) {
        case 1:
            av_get_channel_layout_string(ch_layout, sizeof(ch_layout), 0, AV_CH_LAYOUT_MONO);
            break;
        case 2:
            av_get_channel_layout_string(ch_layout, sizeof(ch_layout), 0, AV_CH_LAYOUT_STEREO);
            break;
        default:
            av_get_channel_layout_string(ch_layout, sizeof(ch_layout), 0, AV_CH_LAYOUT_MONO);
            break;
    }
    av_opt_set(abuffer_ctx, "channel_layout", ch_layout, AV_OPT_SEARCH_CHILDREN);
    av_opt_set(abuffer_ctx, "sample_fmt", av_get_sample_fmt_name(INPUT_FORMAT), AV_OPT_SEARCH_CHILDREN);
    av_opt_set_q(abuffer_ctx, "time_base", (AVRational) {1, (int) sample_rate}, AV_OPT_SEARCH_CHILDREN);
    av_opt_set_int(abuffer_ctx, "sample_rate", sample_rate, AV_OPT_SEARCH_CHILDREN);

    /* Now initialize the filter; we pass NULL options, since we have already
     * set all the options above. */
    /* Now initialize the filter; we pass NULL options, since we have already
     * set all the options above. */
    err = avfilter_init_str(abuffer_ctx, nullptr);
    if (err < 0) {
        fprintf(stderr, "Could not initialize the abuffer filter.\n");
        LOGE("AudioFilter::initFilterGraph 4");
        printError(err);
        isConfiguring = false;
        return err;
    }

    if (preset != EFFECT_PRESET_NONE) {
        /* Create echo filter */
        aecho = avfilter_get_by_name("aecho");
        if (!aecho) {
            fprintf(stderr, "Could not find echo filter.\n");
            LOGE("AudioFilter::initFilterGraph 5");
            isConfiguring = false;
            return AVERROR_FILTER_NOT_FOUND;
        }

        aecho_ctx = avfilter_graph_alloc_filter(filter_graph, aecho, "aecho");
        if (!aecho_ctx) {
            fprintf(stderr, "Could not allocate the aecho instance.\n");
            LOGE("AudioFilter::initFilterGraph 6");
            isConfiguring = false;
            return AVERROR(ENOMEM);
        }

        getEchoOptionString(options_str, sizeof(options_str), preset);
        err = avfilter_init_str(aecho_ctx, options_str);
        if (err < 0) {
            fprintf(stderr, "Could not initialize the aecho filter.\n");
            LOGE("AudioFilter::initFilterGraph 7");
            printError(err);
            isConfiguring = false;
            return err;
        }
        memset(options_str, 0, 1024);

        /* Create equalizer filter */
        anequalizer = avfilter_get_by_name("anequalizer");
        if (!anequalizer) {
            fprintf(stderr, "Could not find anequalizer filter.\n");
            LOGE("AudioFilter::initFilterGraph 19");
            isConfiguring = false;
            return AVERROR_FILTER_NOT_FOUND;
        }

        anequalizer_ctx = avfilter_graph_alloc_filter(filter_graph, anequalizer, "anequalizer");
        if (!anequalizer_ctx) {
            fprintf(stderr, "Could not allocate the anequalizer instance.\n");
            LOGE("AudioFilter::initFilterGraph 20");
            isConfiguring = false;
            return AVERROR(ENOMEM);
        }

        getEqualizerOptionString(options_str, sizeof(options_str), preset);
        err = avfilter_init_str(anequalizer_ctx, options_str);
        if (err < 0) {
            fprintf(stderr, "Could not initialize the anequalizer filter.\n");
            LOGE("AudioFilter::initFilterGraph 21");
            printError(err);
            isConfiguring = false;
            return err;
        }
        memset(options_str, 0, 1024);
    }

    /* Create volume filter. */
    avolume = avfilter_get_by_name("volume");
    if (!avolume) {
        fprintf(stderr, "Could not find the volume filter.\n");
        LOGE("AudioFilter::initFilterGraph 8");
        isConfiguring = false;
        return AVERROR_FILTER_NOT_FOUND;
    }

    volume_ctx = avfilter_graph_alloc_filter(filter_graph, avolume, "volume");
    if (!volume_ctx) {
        fprintf(stderr, "Could not allocate the volume instance.\n");
        LOGE("AudioFilter::initFilterGraph 9");
        isConfiguring = false;
        return AVERROR(ENOMEM);
    }

    /* A different way of passing the options is as key/value pairs in a
     * dictionary. */
    snprintf(options_str, sizeof(options_str), "%f", volume);
    av_dict_set(&options_dict, "volume", options_str, 0);
    err = avfilter_init_dict(volume_ctx, &options_dict);
    av_dict_free(&options_dict);
    if (err < 0) {
        fprintf(stderr, "Could not initialize the volume filter.\n");
        LOGE("AudioFilter::initFilterGraph 10");
        printError(err);
        isConfiguring = false;
        return err;
    }
    memset(options_str, 0, 1024);

    /* Create the aformat filter;
     * it ensures that the output is of the format we want. */
    aformat = avfilter_get_by_name("aformat");
    if (!aformat) {
        fprintf(stderr, "Could not find the aformat filter.\n");
        LOGE("AudioFilter::initFilterGraph 11");
        isConfiguring = false;
        return AVERROR_FILTER_NOT_FOUND;
    }

    aformat_ctx = avfilter_graph_alloc_filter(filter_graph, aformat, "aformat");
    if (!aformat_ctx) {
        fprintf(stderr, "Could not allocate the aformat instance.\n");
        LOGE("AudioFilter::initFilterGraph 12");
        isConfiguring = false;
        return AVERROR(ENOMEM);
    }

    /* A third way of passing the options is in a string of the form
     * key1=value1:key2=value2.... */
    uint32_t channelcount;
    switch (channels) {
        case 1:
            channelcount = AV_CH_LAYOUT_MONO;
            break;
        case 2:
            channelcount = AV_CH_LAYOUT_STEREO;
            break;
        default:
            channelcount = AV_CH_LAYOUT_MONO;
            break;
    }
    snprintf(options_str, sizeof(options_str),
             "sample_fmts=%s:sample_rates=%d:channel_layouts=0x%x",
             av_get_sample_fmt_name(AV_SAMPLE_FMT_S16), sample_rate,
             channelcount);
    err = avfilter_init_str(aformat_ctx, options_str);
    if (err < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not initialize the aformat filter.\n");
        LOGE("AudioFilter::initFilterGraph 13");
        printError(err);
        isConfiguring = false;
        return err;
    }

    /* Finally create the abuffersink filter;
     * it will be used to get the filtered data out of the graph. */
    abuffersink = avfilter_get_by_name("abuffersink");
    if (!abuffersink) {
        fprintf(stderr, "Could not find the abuffersink filter.\n");
        LOGE("AudioFilter::initFilterGraph 14");
        isConfiguring = false;
        return AVERROR_FILTER_NOT_FOUND;
    }

    abuffersink_ctx = avfilter_graph_alloc_filter(filter_graph, abuffersink, "sink");
    if (!abuffersink_ctx) {
        fprintf(stderr, "Could not allocate the abuffersink instance.\n");
        LOGE("AudioFilter::initFilterGraph 15");
        isConfiguring = false;
        return AVERROR(ENOMEM);
    }

    /* This filter takes no options. */
    err = avfilter_init_str(abuffersink_ctx, nullptr);
    if (err < 0) {
        fprintf(stderr, "Could not initialize the abuffersink instance.\n");
        LOGE("AudioFilter::initFilterGraph 16");
        printError(err);
        isConfiguring = false;
        return err;
    }

    /* Connect the filters;
     * in this simple case the filters just form a linear chain. */
    if (preset != EFFECT_PRESET_NONE) {
        err = avfilter_link(abuffer_ctx, 0, aecho_ctx, 0);
        if (err >= 0)
            err = avfilter_link(aecho_ctx, 0, anequalizer_ctx, 0);
        if (err >= 0)
            err = avfilter_link(anequalizer_ctx, 0, volume_ctx, 0);
        if (err >= 0)
            err = avfilter_link(volume_ctx, 0, aformat_ctx, 0);
        if (err >= 0)
            err = avfilter_link(aformat_ctx, 0, abuffersink_ctx, 0);
    } else {
        err = avfilter_link(abuffer_ctx, 0, volume_ctx, 0);
        if (err >= 0)
            err = avfilter_link(volume_ctx, 0, aformat_ctx, 0);
        if (err >= 0)
            err = avfilter_link(aformat_ctx, 0, abuffersink_ctx, 0);
    }
    if (err < 0) {
        fprintf(stderr, "Error connecting filters\n");
        LOGE("AudioFilter::initFilterGraph 17");
        printError(err);
        isConfiguring = false;
        return err;
    }

    /* Configure the graph. */
    err = avfilter_graph_config(filter_graph, nullptr);
    if (err < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error configuring the filter graph\n");
        LOGE("AudioFilter::initFilterGraph 18");
        printError(err);
        isConfiguring = false;
        return err;
    }

    *graph = filter_graph;
    *src = abuffer_ctx;
    *sink = abuffersink_ctx;

    isConfiguring = false;

    LOGD(" --------- AudioFilter::initFilterGraph success ---------");
    return 0;
}
