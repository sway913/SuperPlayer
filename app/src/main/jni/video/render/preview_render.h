//
// Created by Lvlingling on 2020/8/14.
//

#ifndef SUPERPLAYER_PREVIEW_RENDER_H
#define SUPERPLAYER_PREVIEW_RENDER_H


#include "irender.h"

class PreviewRender : public IRender {

public:

    void onSurfaceCreate(jobject surface, int w, int h) override;

    void onDraw() override;

    void onSurfaceDestroy() override;

    void setSource(Source *s) override;

    void setFilter(const std::shared_ptr<VFilter> &f) override;

    void start(JNIEnv *env, const char *video_path) override;

    void stop() override;

    void setState(bool isPause) override;

    virtual ~PreviewRender();

private:

    Source *source{nullptr};

};


#endif //SUPERPLAYER_PREVIEW_RENDER_H
