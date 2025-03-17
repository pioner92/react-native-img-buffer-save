#import "ImgBufferSave.h"

#import <React/RCTBridge+Private.h>
#import <React/RCTBridge.h>
#import <React/RCTUtils.h>
#import <ReactCommon/RCTTurboModule.h>
#import <jsi/jsi.h>
#include <filesystem>
#include <iostream>

@implementation ImgBufferSave
RCT_EXPORT_MODULE()

+ (BOOL)requiresMainQueueSetup {
  return YES;
}

using namespace facebook;
using namespace std;

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
  NSLog(@"Installing JSI bindings for save-image-to-gallery...");
  RCTBridge* bridge = [RCTBridge currentBridge];
  RCTCxxBridge* cxxBridge = (RCTCxxBridge*)bridge;

  if (cxxBridge == nil) {
    return @false;
  }

  auto jsiRuntime = (jsi::Runtime*)cxxBridge.runtime;
  if (jsiRuntime == nil) {
    return @false;
  }

  auto jsCallInvoker = bridge.jsCallInvoker;

  sh::ImgBufferSave::initialize(*jsiRuntime);

  return @true;
}

namespace sh::ImgBufferSave {
void initialize(jsi::Runtime& rt) {
  jsi::Function fn = jsi::Function::createFromHostFunction(
      rt, jsi::PropNameID::forAscii(rt, "saveImageToGallery"), 1,
      [](jsi::Runtime& rt, const jsi::Value& thisVal, const jsi::Value* args,
         size_t count) {
        if (!args[0].isObject()) {
          throw jsi::JSError(
              rt, "[SaveImageToGallery] argument must be an ArrayBuffer");
        }

        jsi::Object obj = args[0].asObject(rt);

        if (!obj.isArrayBuffer(rt)) {
          throw jsi::JSError(
              rt, "[SaveImageToGallery] argument must be an ArrayBuffer");
        }

        jsi::ArrayBuffer buffer = obj.getArrayBuffer(rt);

        __block NSData* imageData = [NSData dataWithBytes:buffer.data(rt)
                                                   length:buffer.size(rt)];

        dispatch_async(
            dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
              UIImage* image = [UIImage imageWithData:imageData];
              if (image) {
                UIImageWriteToSavedPhotosAlbum(image, nil, nil, nil);

#ifdef DEBUG
                NSLog(@"[SaveImageToGallery] Image saved");
#endif
              }
              imageData = nil;
            });

        return true;
      });

  rt.global().setProperty(rt, "saveImageToGallery", std::move(fn));
}
}
@end
