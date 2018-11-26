# Visual Object Recognition Using OpenCV

## Setup

1. Install latest version of android sdk and cmake
2. Download Android NDK 17c from <https://developer.android.com/ndk/downloads/older_releases.html> and put it in android SDK folder
3. Download [OpenCV Android SDK 3.4.3](https://sourceforge.net/projects/opencvlibrary/files/opencv-android/3.4.3/)
4. replace several places in the code from OpenCV:
  - replace the line 8 in `app/src/CMakeList.txt` to `include_directories(<YOUR_OPENCV_SDK>/sdk/native/jni/include)`
  - remove `app/src/main/jniLibs` and create a symlink named jniLibs in app/src/main that points to `YOUR_OPENCV_SDK/sdk/native/libs`
