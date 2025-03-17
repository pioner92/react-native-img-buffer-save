import { Text, View, StyleSheet,TouchableOpacity } from 'react-native';
import { saveImageToGallery } from 'react-native-img-buffer-save';



export default function App() {
  const saveImg = () => {

    const img = Uint8Array.from([0, 1, 2, 3, 4, 5, 6, 7, 8, 9]) //  some image;

    saveImageToGallery(img); // Uint8Array
    // or
    saveImageToGallery(img.buffer); // ArrayBuffer
  }

  return (
    <View style={styles.container}>
      <TouchableOpacity onPress={saveImg}>
      <Text>Save IMG to gallery</Text>
      </TouchableOpacity>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
});
