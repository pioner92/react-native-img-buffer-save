import { NativeModules } from 'react-native'


declare global {
  type saveImageToGallery = (buffer: ArrayBuffer) => void

  var saveImageToGallery: saveImageToGallery
}


let __saveImageToGallery = global.saveImageToGallery

if (!__saveImageToGallery) {
  if (NativeModules.ImgBufferSave?.install) {
    NativeModules.ImgBufferSave.install() // Вызываем native install метод
    __saveImageToGallery = global.saveImageToGallery // Сохраняем глобальную ссылку на объект SqlDb
    console.log('✅ react-native-img-buffer-save initialized successfully')
  }
}
export const saveImageToGallery = (buff:ArrayBuffer | Uint8Array)=> {
  if (buff instanceof Uint8Array) {
    __saveImageToGallery(buff.buffer);
  } else if (buff instanceof ArrayBuffer) {
    __saveImageToGallery(buff);
  } else {
    throw new Error('Invalid argument type; must be ArrayBuffer or Uint8Array');
  }
}

