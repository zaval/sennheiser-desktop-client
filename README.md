# Sennheiser Desktop Client

This is an unofficial desktop client of Sennheiser Smart Control app.

Currently, this app works with Momentum 4 headphones.

## Requirements

* Cmake >= 3.20
* C++ compiler supporting C++11 (GCC >= 4.8, Clang >= 3.0, MSVC >= 19)
* Qt 6
* Python (needed for class generation from the json schema)

## Build

```shell
cmake -S . -B build
cmake --build build -- -j
```

If you have Qt in some custom path, for example installed from the qt installer 
add `-DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/os`

## Add support for the new model

To add support for the new device or update the current device:

1. Download the Sennheiser Smart Control APK from Google Play
2. Decompile resources from the APK
    ```shell
    java -jar apktool.jar d -s com.sennheiser.control.apk
    ```
3. Find the json file (e.g. m4.json for MONENTUM 4) in folder **resources/assets/app** and copy this file into the gaiaV3 folder
4. If the file name is not m4.json, update `${CMAKE_CURRENT_SOURCE_DIR}/m4.json` to the correct name the **gaiaV3/CMakeLists.txt** file 
5. Clean, rerun cmake and rebuild the project
