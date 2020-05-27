package com.smzh.superplayer;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;

public class SuperPlayer implements Handler.Callback {

    private static final int MESSAGE_PREPARE = 1;
    private static final int MESSAGE_START = 2;
    private static final int MESSAGE_RESUME = 3;
    private static final int MESSAGE_PAUSE = 4;
    private static final int MESSAGE_STOP = 5;
    private static final int MESSAGE_SEEK = 6;
    private static final int MESSAGE_ECHO = 7;
    private static final int MESSAGE_FILTER = 8;
    private static final int MESSAGE_VOLUME = 9;
    private static final int MESSAGE_PREPARE_MERGE = 10;
    private static final int MESSAGE_STOP_MERGE = 11;
    private static final int MESSAGE_PITCH = 12;
    private static final int MESSAGE_START_MERGER = 13;

    private Handler audioHandler;
    private AudioJni audioJni;

    private static class SuperPlayerHolder {
        private static SuperPlayer INSTANCE = new SuperPlayer();
    }


    public static SuperPlayer getInstance() {
        return SuperPlayerHolder.INSTANCE;
    }


    private SuperPlayer() {
        HandlerThread audioThread = new HandlerThread("audio_thread");
        audioThread.start();
        audioHandler = new Handler(audioThread.getLooper(), this);
        audioJni = new AudioJni();
    }

    @Override
    public boolean handleMessage(Message msg) {
        switch (msg.what) {
            case MESSAGE_PREPARE:
                EventPath event = (EventPath) msg.obj;
                audioJni.prepare(event.isRecordMode, event.sample, event.musicPath, event.guidePath, event.vocalPath);
                break;
            case MESSAGE_START:
                audioJni.start();
                break;
            case MESSAGE_RESUME:
                audioJni.resume();
                break;
            case MESSAGE_PAUSE:
                audioJni.pause();
                break;
            case MESSAGE_STOP:
                audioJni.stop();
                break;
            case MESSAGE_SEEK:
                audioJni.seek((Long) msg.obj);
                break;
            case MESSAGE_ECHO:
                audioJni.setEcho((Boolean) msg.obj);
                break;
            case MESSAGE_FILTER:
                audioJni.setFilter((Integer) msg.obj);
                break;
            case MESSAGE_PITCH:
                audioJni.setPitch((Integer) msg.obj);
                break;
            case MESSAGE_VOLUME:
                audioJni.setVolume((Float) msg.obj, msg.arg1);
                break;
            case MESSAGE_PREPARE_MERGE:
                EventPath event1 = (EventPath) msg.obj;
                audioJni.prepareMerge(event1.vocalPath, event1.musicPath, event1.guidePath);
                break;
            case MESSAGE_START_MERGER:
                EventMerge eventMerge = (EventMerge) msg.obj;
                audioJni.startMerge(eventMerge.vocalVolume, eventMerge.vocalFilter, eventMerge.musicVolume, eventMerge.musicPitch);
            case MESSAGE_STOP_MERGE:
                audioJni.stopMerge();
                break;
            default:
                break;
        }


        return false;
    }


    public void prepare(boolean isRecordMode, int sample, String musicPath, String guidePath, String vocaPath) {
        EventPath event = new EventPath(isRecordMode, sample, musicPath, guidePath, vocaPath);
        audioHandler.obtainMessage(MESSAGE_PREPARE, event).sendToTarget();
    }

    public void start() {
        audioHandler.obtainMessage(MESSAGE_START).sendToTarget();
    }

    public void resume() {
        audioHandler.obtainMessage(MESSAGE_RESUME).sendToTarget();
    }

    public void pause() {
        audioHandler.obtainMessage(MESSAGE_PAUSE).sendToTarget();
    }

    public void stop() {
        audioHandler.obtainMessage(MESSAGE_STOP).sendToTarget();
    }


    public void seek(long millis) {
        audioHandler.obtainMessage(MESSAGE_SEEK, millis).sendToTarget();
    }

    public void setEcho(boolean isEcho) {
        audioHandler.obtainMessage(MESSAGE_ECHO, isEcho).sendToTarget();
    }


    public void setFilter(int type) {
        audioHandler.obtainMessage(MESSAGE_FILTER, type).sendToTarget();
    }

    public void setVolume(float volume, int track) {
        audioHandler.obtainMessage(MESSAGE_VOLUME, track, 0, volume).sendToTarget();
    }

    public void setPitch(int pitch) {
        audioHandler.obtainMessage(MESSAGE_PITCH, pitch).sendToTarget();
    }

    public void prepareMerge(String vocalPath, String musicPath, String mixPath) {
        EventPath event = new EventPath(false, 0, musicPath, mixPath, vocalPath);
        audioHandler.obtainMessage(MESSAGE_PREPARE_MERGE, event).sendToTarget();
    }

    public void startMerge(int vocalVolume, int vocalFilter, int musicVolume, float musicPitch) {
        EventMerge event = new EventMerge(vocalVolume, vocalFilter, musicVolume, musicPitch);
        audioHandler.obtainMessage(MESSAGE_START_MERGER, event).sendToTarget();
    }

    public void stopMerge() {
        audioHandler.obtainMessage(MESSAGE_STOP_MERGE).sendToTarget();
    }

    public long getTotalMs() {
        return audioJni.getTotalMs();
    }

    public long getCurrentMs() {
        return audioJni.getCurrentMs();
    }

    public int getMergeProgress() {
        return audioJni.getMergeProgress();
    }

    public void setDataProcessListener(AudioJni.DataProcessListener listener) {
        audioJni.setProcessListener(listener);
    }

    public void removeDataProcessListener(AudioJni.DataProcessListener listener) {
        audioJni.removeProcessListener(listener);
    }

    private class EventPath {
        boolean isRecordMode;
        int sample;
        String musicPath;
        String guidePath;
        String vocalPath;

        EventPath(boolean isRecordMode, int sample, String musicPath, String guidePath, String vocalPath) {
            this.isRecordMode = isRecordMode;
            this.sample = sample;
            this.musicPath = musicPath;
            this.guidePath = guidePath;
            this.vocalPath = vocalPath;
        }
    }

    private class EventMerge {

        int vocalVolume;
        int vocalFilter;
        int musicVolume;
        float musicPitch;

        public EventMerge(int vocalVolume, int vocalFilter, int musicVolume, float musicPitch) {
            this.vocalVolume = vocalVolume;
            this.vocalFilter = vocalFilter;
            this.musicVolume = musicVolume;
            this.musicPitch = musicPitch;
        }
    }
}
