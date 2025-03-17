cmake_minimum_required(VERSION 3.4.1)
project(imgbuffersave)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)


add_library(imgbuffersave  SHARED
            cpp-adapter.cpp
)


find_package(ReactAndroid REQUIRED CONFIG)

find_package(fbjni REQUIRED CONFIG)

target_include_directories(
        imgbuffersave PRIVATE
        "${NODE_MODULES_DIR}/react-native/React"
        "${NODE_MODULES_DIR}/react-native/React/Base"
        "${NODE_MODULES_DIR}/react-native/ReactCommon"
        "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi"
        "${NODE_MODULES_DIR}/react-native/ReactCommon/callinvoker"
        "${NODE_MODULES_DIR}/react-native/ReactAndroid/src/main/jni/react/turbomodule"
        "${NODE_MODULES_DIR}/react-native/ReactCommon/runtimeexecutor"
)

target_link_libraries(imgbuffersave
        ReactAndroid::jsi
        log
        ReactAndroid::reactnative
        fbjni::fbjni
)
