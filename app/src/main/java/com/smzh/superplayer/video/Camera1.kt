package com.smzh.superplayer.video

import android.graphics.ImageFormat
import android.graphics.SurfaceTexture
import android.hardware.Camera
import com.smzh.superplayer.player.SuperPlayer

@Suppress("DEPRECATION")
class Camera1 : SuperCamera(), Camera.PreviewCallback {

    private var camera: Camera? = null
    private lateinit var surfaceTexture: SurfaceTexture
    private var cameraId = Camera.CameraInfo.CAMERA_FACING_FRONT
    private val parameter = IntArray(3)

    override fun open(): IntArray {
        camera = Camera.open(cameraId)
        parameter[0] = cameraId
        try {
            camera?.parameters = camera?.parameters?.apply {
                setPreviewSize(1920, 1080)
                if (supportedFocusModes.contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO)) {
                    focusMode = Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO
                }
                previewFormat = ImageFormat.NV21
                parameter[1] = 1080
                parameter[2] = 1920
            }
            camera?.autoFocus { _, _ ->

            }
            camera?.setPreviewCallbackWithBuffer(this)
            camera?.addCallbackBuffer(ByteArray(1920 * 1080 * ImageFormat.getBitsPerPixel(ImageFormat.NV21) / 8))
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return parameter
    }

    //egl thread call
    override fun updateImage(): Long {
        try {
            surfaceTexture.updateTexImage()
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return surfaceTexture.timestamp
    }

    override fun startPreview(textureId: Int) {
        surfaceTexture = SurfaceTexture(textureId)
        camera?.setPreviewTexture(surfaceTexture)
        camera?.startPreview()
        surfaceTexture.setOnFrameAvailableListener {
            SuperPlayer.instance.onFrameAvailable()
        }
    }

    override fun switchCamera(): IntArray {
        if (Camera.getNumberOfCameras() > 1) {
            cameraId = if (isFront()) {
                Camera.CameraInfo.CAMERA_FACING_BACK
            } else {
                Camera.CameraInfo.CAMERA_FACING_FRONT
            }
        }
        close()
        return open()
    }

    override fun isFront(): Boolean {
        return cameraId == Camera.CameraInfo.CAMERA_FACING_FRONT
    }

    override fun onPreviewFrame(data: ByteArray?, camera: Camera?) {
        data?.let {
            SuperPlayer.instance.encodePreviewData(data)
        }
        camera?.addCallbackBuffer(data)
    }

    override fun close() {
        camera?.stopPreview()
        camera?.release()
        camera = null
    }
}