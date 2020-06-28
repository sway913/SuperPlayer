package com.smzh.superplayer.video

import android.graphics.SurfaceTexture
import android.hardware.Camera
import com.smzh.superplayer.player.SuperPlayer

@Suppress("DEPRECATION")
class Camera1 : SuperCamera() {

    private var camera: Camera? = null
    private lateinit var surfaceTexture: SurfaceTexture
    private var cameraId = Camera.CameraInfo.CAMERA_FACING_FRONT
    private val parameter = IntArray(3)

    override fun open(): IntArray {
        cameraId = if (Camera.getNumberOfCameras() > 1) {
            Camera.CameraInfo.CAMERA_FACING_FRONT
        } else {
            Camera.CameraInfo.CAMERA_FACING_BACK
        }
        camera = Camera.open(cameraId)
        parameter[0] = cameraId
        try {
            camera?.parameters = camera?.parameters?.apply {
                setPreviewSize(1920, 1080)
                if (supportedFocusModes.contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO)) {
                    focusMode = Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO
                }
                parameter[1] = 1080
                parameter[2] = 1920
            }
            camera?.autoFocus { _, _ ->

            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return parameter
    }

    //egl thread call
    override fun updateImage() {
        try {
            surfaceTexture.updateTexImage()
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    override fun startPreview(textureId: Int) {
        surfaceTexture = SurfaceTexture(textureId)
        camera?.setPreviewTexture(surfaceTexture)
        camera?.startPreview()
        surfaceTexture.setOnFrameAvailableListener {
            SuperPlayer.instance.onFrameAvailable()
        }
    }

    override fun switchCamera() {
        if (Camera.getNumberOfCameras() > 1) {
            cameraId = if (isFront()) {
                Camera.CameraInfo.CAMERA_FACING_BACK
            } else {
                Camera.CameraInfo.CAMERA_FACING_FRONT
            }
        }
    }

    override fun isFront(): Boolean {
        return cameraId == Camera.CameraInfo.CAMERA_FACING_FRONT
    }

    override fun close() {
        camera?.stopPreview()
        camera?.release()
        camera = null
    }
}