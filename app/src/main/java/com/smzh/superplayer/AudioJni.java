package com.smzh.superplayer;

import java.util.Vector;

public class AudioJni {

    static {
        System.loadLibrary("music");
    }

    //声明周期方法请按顺序调用
    public native void prepare(boolean isRecordMode, int sample, String musicPath, String guidePath, String vocalPath);

    public native void start();

    public native void resume();

    public native void pause();

    public native void stop();

    public native void seek(long millis);

    public native void setEcho(boolean isEcho);

    public native void setFilter(int type);

    public native void setVolume(float volume, int track);

    public native void setPitch(int pitch);

    public native long getTotalMs();

    public native long getCurrentMs();

    public native void prepareMerge(String vocalPath, String musicPath, String mixPath);

    public native void startMerge(int vocalVolume,int filter,int musicVolume,float musicPitch);

    public native void stopMerge();

    public native int getMergeProgress();


    private static Vector<DataProcessListener> listeners = new Vector<>();

    public void setProcessListener(DataProcessListener listener) {
        if (!listeners.contains(listener)) {
            listeners.add(listener);
        }
    }

    public void removeProcessListener(DataProcessListener listener) {
        listeners.remove(listener);
    }


    public void onDataReady() {
        for (DataProcessListener listener : listeners) {
            listener.onDataReady();
        }
    }

    public void onCompleted() {
        for (DataProcessListener listener : listeners) {
            listener.onCompleted();
        }
    }

    public void onError() {
        for (DataProcessListener listener : listeners) {
            listener.onError();
        }
    }

    public interface DataProcessListener {

        void onDataReady();

        void onCompleted();

        void onError();

    }

}
