# rprframework
Project with CMake build system to start with RadeonProRender SDK and RadeonProRender Image Processing Library

# How to build

## On Linux

    cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -GNinja  -DFORCE_COLORED_OUTPUT=true ..

## On Windows (vs 2022)

Prepare and install prerequisited
1. Install [cmake](https://cmake.org/download)
2. Install and compile boost 
  - Download [boost](https://www.boost.org/users/download)
  - Unzip somewhere
  - cd to unziped directory
  - Run ```bootstrap.bat```
  - run ```b2```
  - run ```b2 install --prefix=c:/libs/boost```
  - Setup environment variable ```BOOST_ROOT``` to ```c:/libs/boost```
3. Install and compile google tests
   - Download [gtest](https://github.com/google/googletest)
   - Unzip somewhere
   - Go to unziped directory and create ```build``` directory
   - Create VS solution file with command
      ```cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX="c:/libs/gtest" -Dgtest_force_shared_crt=true ..
```
   - Open solution file in studio and compile
      - After compile, rigth click on INSTALL project and select 
	  *Build* from context menu
   - Setup environment variable ```GTEST_ROOT``` to ```c:/libs/gtest```
4. Prepare [RadeonProRenderSDK](https://github.com/GPUOpen-LibrariesAndSDKs/RadeonProRenderSDK)
   - Clone RadeonProRenderSDK
     ```git clone https://github.com/GPUOpen-LibrariesAndSDKs/RadeonProRenderSDK.git c:/libs/RadeonProRenderSDK```
   - Setup environment variable ```RPR_SDK_ROOT``` to cloned directory ```c:/libs/RadeonProRenderSDK```
5. 
    cmake -G "Visual Studio 17 2022" -A x64 ..
