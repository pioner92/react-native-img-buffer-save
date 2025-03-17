package com.imgbuffersave
import android.content.ContentValues
import android.provider.MediaStore
import android.util.Log
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactContextBaseJavaModule
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.common.annotations.FrameworkAPI
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

@OptIn(FrameworkAPI::class)
class ImgBufferSaveModule internal constructor(val context: ReactApplicationContext) :
    ReactContextBaseJavaModule(context) {

    companion object {

        const val NAME = "ImgBufferSave"

        init {
            System.loadLibrary("imgbuffersave")
        }

        lateinit var instance: ImgBufferSaveModule private set


        @OptIn(FrameworkAPI::class)
        @JvmStatic
        fun addImageToGalleryAsync(fileName: String, imageData: ByteArray) {
            CoroutineScope(Dispatchers.IO).launch {
                addImageToGallery(fileName, imageData)
            }
        }

        @OptIn(FrameworkAPI::class)
        @JvmStatic
        fun addImageToGallery(fileName: String, imageData: ByteArray) {
            try {
                val contentValues = ContentValues().apply {
                    put(MediaStore.Images.Media.DISPLAY_NAME, fileName)
                    put(MediaStore.Images.Media.MIME_TYPE, "image/png")
                    put(MediaStore.Images.Media.RELATIVE_PATH, "Pictures/")
                    put(MediaStore.Images.Media.IS_PENDING, 1)
                }

                val resolver = instance.reactContext.contentResolver
                val uri =
                    resolver.insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, contentValues)

                uri?.let {
                    resolver.openOutputStream(it)?.use { outputStream ->
                        outputStream.write(imageData)
                    }

                    contentValues.clear()
                    contentValues.put(MediaStore.Images.Media.IS_PENDING, 0)
                    resolver.update(uri, contentValues, null, null)

                    if (BuildConfig.DEBUG) {
                        Log.d(NAME, "✅ saved to the gallery: $fileName")
                    }
                }

            } catch (e: Exception) {
                Log.e(NAME, "❌ Error add to the gallery", e)
            }
        }

        @OptIn(FrameworkAPI::class)
        @JvmStatic
        external fun nativeInstall(jsiRuntimePointer: Long, jsCallInvoker: CallInvokerHolderImpl)

    }

    private val reactContext = context

    init {
        instance = this
    }


    override fun getName(): String {
        return NAME
    }

    @OptIn(FrameworkAPI::class)
    @ReactMethod(isBlockingSynchronousMethod = true)
    fun install(): Boolean {
        if (BuildConfig.DEBUG) {
            Log.d(NAME, "install() called")
        }


        val callInvokerHolder =
            reactContext.catalystInstance.jsCallInvokerHolder as CallInvokerHolderImpl

        nativeInstall(
            context.javaScriptContextHolder!!.get(),
            callInvokerHolder
        )

        return true
    }
}
