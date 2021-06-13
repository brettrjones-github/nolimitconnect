
### BUILD TYPE
CONFIG(debug, debug|release){
    BUILD_TYPE=Debug
    DEBUG_LIBNAME_APPEND=_d
}

CONFIG(release, debug|release){
    BUILD_TYPE=Release
    DEBUG_LIBNAME_APPEND=
}

#define this next line to make all android types builds
#    ANDROID_ABIS=armeabi-v7a armeabi-v8a x86 x86_64

### HOST OS COMPILE AND TARGET OS

win32:TARGET_OS_NAME = Windows
unix:!android: TARGET_OS_NAME = Linux
android: TARGET_OS_NAME = Android
macx: TARGET_OS_NAME = Apple

COMPILE_HOST_NAME=$${QMAKE_HOST.os}

contains( COMPILE_HOST_NAME, Linux) {
    COMPILE_HOST_OS = Linux

    PRO_MKDIR_CMD = mkdir -p
    PRO_COPY_CMD = cp -f
    PRO_MOVE_CMD = cp -f
}

contains( COMPILE_HOST_NAME, Android ) {
    COMPILE_HOST_OS = Android

    PRO_MKDIR_CMD = mkdir -p
    PRO_COPY_CMD = cp -f
    PRO_MOVE_CMD = cp -f
}

contains( COMPILE_HOST_NAME, Windows ) {
    COMPILE_HOST_OS = Windows

    PRO_MKDIR_CMD = mkdir
    PRO_COPY_CMD = copy /Y
    PRO_MOVE_CMD = move /Y
}


win32:{
    DEFINES += TARGET_OS_WINDOWS
    DEFINES += WIN64
    DEFINES += _WIN64
    DEFINES += TARGET_CPU_64BIT
    DEFINES += TARGET_CPU_X86_64
    TARGET_ARCH_NAME=x86_64
    TARGET_CPU_BITS=64
    TARGET_ENDIAN_LITTLE=1
    TARGET_ENDIAN_BIG=0
}

unix:!android:{
    DEFINES += TARGET_OS_LINUX
    DEFINES += TARGET_POSIX
    DEFINES += TARGET_CPU_64BIT
    DEFINES += TARGET_CPU_X86_64
    TARGET_ARCH_NAME=x86_64
    TARGET_CPU_BITS=64
    TARGET_ENDIAN_LITTLE=1
    TARGET_ENDIAN_BIG=0
}

android:{
    # this next line is all possible android abis
    #ANDROID_ABIS += armeabi-v7a arm64-v8a x86_64 x86
    DEFINES += TARGET_OS_ANDROID
    DEFINES += TARGET_POSIX
    ANDROID_ARM64 = 0
    ANDROID_ARMv7 = 0
    ANDROID_x86 = 0
    ANDROID_x86_64 = 0

    message(ANDROID_TARGET $${ANDROID_TARGET_ARCH})

    equals(ANDROID_TARGET_ARCH,arm64-v8a) {
        DEFINES += TARGET_CPU_64BIT
        DEFINES += TARGET_CPU_ARM
        ANDROID_ARM64 = 1
        TARGET_ARCH_NAME=armeabi-v8a
        TARGET_CPU_BITS=64
        TARGET_ENDIAN_LITTLE=1
        TARGET_ENDIAN_BIG=0
    }

    equals(ANDROID_TARGET_ARCH,armeabi-v7a) {
        DEFINES += TARGET_CPU_32BIT
        DEFINES += TARGET_CPU_ARM
        ANDROID_ARMv7 = 1
        TARGET_ARCH_NAME=armeabi-v7a
        TARGET_CPU_BITS=32
        TARGET_ENDIAN_LITTLE=1
        TARGET_ENDIAN_BIG=0
    }

    equals(ANDROID_TARGET_ARCH,x86_64) {
        DEFINES += TARGET_CPU_64BIT
        DEFINES += TARGET_CPU_X86_64
        ANDROID_x86_64 = 1
        TARGET_ARCH_NAME=x86_64
        TARGET_CPU_BITS=64
        TARGET_ENDIAN_LITTLE=1
        TARGET_ENDIAN_BIG=0
    }
}

BUILD_STATIC_LIBS_DIR=$$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
BUILD_SHARED_LIB_DIR=$$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}

android:{
    DEST_SHARED_LIBS_DIR = $$PWD/bin-Android/libs/$${TARGET_ARCH_NAME}/
    DEST_EXE_DIR = $$PWD/bin-Android/libs/$${TARGET_ARCH_NAME}/
    DEST_PYTHON_EXE_DIR = $$PWD/bin-Android/
    DEST_PYTHON_DLL_DIR = $$PWD/bin-Android/libs/$${TARGET_ARCH_NAME}/Dlls/
    DEST_PYTHON_LIB_DIR = $$PWD/bin-Android/libs/$${TARGET_ARCH_NAME}/Lib/
}

win32:{
    DEST_SHARED_LIBS_DIR = $$PWD/bin-Windows/
    DEST_EXE_DIR = $$PWD/bin-Windows/
    DEST_PYTHON_EXE_DIR = $$PWD/bin-Windows/
    DEST_PYTHON_DLL_DIR = $$PWD/bin-Windows/assets/kodi/system/Python/DLLs/
    DEST_PYTHON_LIB_DIR = $$PWD/bin-Windows/assets/kodi/system/Python/Lib/
}

unix:!android:{

    DEST_SHARED_LIBS_DIR = $$PWD/bin-Linux/
    DEST_EXE_DIR = $$PWD/bin-Linux/
    DEST_PYTHON_EXE_DIR = $$PWD/bin-Linux/
    DEST_PYTHON_DLL_DIR = $$PWD/bin-Linux/assets/kodi/system/Python/DLLs
    DEST_PYTHON_LIB_DIR = $$PWD/bin-Linux/assets/kodi/system/Python/Lib
}

android: {
    TARGET_ARCH_APPEND = _$${TARGET_ARCH_NAME}
    STATIC_LIB_APPEND = $${TARGET_ARCH_NAME}.a
    SHARED_LIB_APPEND = _$${TARGET_ARCH_NAME}.so
}

win32: {
    TARGET_ARCH_APPEND =
    STATIC_LIB_APPEND =  .lib
    SHARED_LIB_APPEND =  .dll
}

unix:!android: {
    TARGET_ARCH_APPEND =
    STATIC_LIB_APPEND = $${DEBUG_LIBNAME_APPEND}.a
    SHARED_LIB_APPEND = $${DEBUG_LIBNAME_APPEND}.so
}


contains( TARGET_CPU_BITS, 64 ) {
    message(Building $${BUILD_TYPE} $${TARGET_OS_NAME} 64 bit $${TARGET_ARCH_NAME} )
}

contains( TARGET_CPU_BITS, 32 ) {
    message(Building $${BUILD_TYPE} $${TARGET_OS_NAME} 32 bit $${TARGET_ARCH_NAME} )
}

