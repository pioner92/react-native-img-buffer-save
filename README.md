# react-native-img-buffer-save

A fast and lightweight React Native library built with JSI that allows you to save images directly to the device's gallery. Supports saving images from `Uint8Array` or `ArrayBuffer` buffers.

## Installation

### Using Yarn

```bash
yarn add react-native-img-buffer-save
```

### Using NPM

```bash
npm install react-native-img-buffer-save
```

### iOS Specific

After installation, don't forget to install CocoaPods dependencies:

```bash
cd ios && pod install
```

## Usage Example

```tsx
import { View, Text, TouchableOpacity, StyleSheet } from 'react-native';
import { saveImageToGallery } from 'react-native-img-buffer-save';

export default function App() {
  const saveImg = () => {
    const imgBytes = Uint8Array.from([0, 1, 2, 3, 4, 5, 6, 7, 8, 9]); // some image;

    saveImageToGallery(img); // Accepts Uint8Array
    // or
    saveImageToGallery(img.buffer); // Accepts ArrayBuffer
  };

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
    justifyContent: 'center',
    alignItems: 'center',
  },
});
```

## Notes
- The library leverages JSI for high performance and low overhead.
- Supports saving images from both `Uint8Array` and `ArrayBuffer`.
- Currently works with both Android and iOS platforms.

---

Feel free to open issues or contribute!


## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
