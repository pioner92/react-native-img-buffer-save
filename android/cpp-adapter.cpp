#include <jni.h>
#include "jsi/jsi.h"
#include <android/log.h>
#include <ReactCommon/CallInvoker.h>
#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <fbjni/detail/Registration.h>
#include <typeinfo>
#include <fstream>
#include <vector>
#include <sys/stat.h>


using namespace facebook;

struct ImgBufferSaveModuleBridge : jni::JavaClass<ImgBufferSaveModuleBridge> {
public:
    static constexpr auto kJavaDescriptor = "Lcom/imgbuffersave/ImgBufferSaveModule;";

    static void registerNatives() {
        javaClassStatic()->registerNatives({
                                                   makeNativeMethod("nativeInstall",
                                                    ImgBufferSaveModuleBridge::nativeInstall)
                                           });
    }

private:
    static void nativeInstall(
            jni::alias_ref<jni::JObject> thiz,
            jlong jsiRuntimePointer,
            jni::alias_ref<react::CallInvokerHolder::javaobject> jsCallInvokerHolder
    ) {

        jsi::Runtime *runtime_ptr = reinterpret_cast<jsi::Runtime *>(jsiRuntimePointer);

        auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();

        auto saveImageToGallery = jsi::Function::createFromHostFunction(
                *runtime_ptr, jsi::PropNameID::forAscii(*runtime_ptr, "saveImageToGallery"), 1,
                [](jsi::Runtime &runtime, const jsi::Value &thisValue,
                   const jsi::Value *arguments, size_t count) -> jsi::Value {

                    if (count < 1 || !arguments[0].isObject()) {
                        throw jsi::JSError(runtime, "[SaveImageToGallery] argument must be an ArrayBuffer");
                    }

                    jsi::Object obj = arguments[0].asObject(runtime);

                    if (!obj.isArrayBuffer(runtime)) {
                        throw jsi::JSError(runtime,
                                           "[SaveImageToGallery] argument must be an ArrayBuffer");
                    }

                    auto buffer = obj.getArrayBuffer(runtime);
                    auto *data = reinterpret_cast<uint8_t *>(buffer.data(runtime));
                    size_t length = buffer.size(runtime);

                    std::string fileName = "INTCH_PROFILE_QR_" + std::to_string(time(nullptr)) + ".png";

                    auto javaClass = jni::findClassStatic(
                            "com/imgbuffersave/ImgBufferSaveModule");
                    auto method = javaClass->getStaticMethod<void(jni::alias_ref<jni::JString>,
                                                                  jni::alias_ref<jni::JPrimitiveArray<jbyteArray>>)>(
                            "addImageToGalleryAsync");

                    jni::local_ref<jni::JString> jFileName = jni::make_jstring(fileName);

                    auto jByteArray = jni::JPrimitiveArray<jbyteArray>::newArray(
                            static_cast<jsize>(length));
                    jByteArray->setRegion(0, static_cast<jsize>(length),
                                          reinterpret_cast<const jbyte *>(data));

                    method(javaClass, jFileName, jByteArray);

                    return true;
                });

        runtime_ptr->global().setProperty(*runtime_ptr, "saveImageToGallery",
                                          std::move(saveImageToGallery));
    }
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    return jni::initialize(vm, [] { ImgBufferSaveModuleBridge::registerNatives(); });
}