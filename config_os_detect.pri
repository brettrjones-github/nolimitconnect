
COMPILE_HOST_OS = UNKNOWN_HOST
COPY_KEYWORD = cp
MOVE_KEYWORD = mv

COMPILE_HOST_NAME=$${QMAKE_HOST.os}
message(compile host is $${COMPILE_HOST_NAME})

#NOTE .. the { must come right after the contains() or will alwayse execute what is in the brackes
contains( COMPILE_HOST_NAME, Windows ) {
    COMPILE_HOST_OS = Windows
    COPY_KEYWORD = copy
    MOVE_KEYWORD = move

    DETECTED_BUILD_DATE=$$system(date /t)
    BUILD_DATE
#    DEFINES += BUILD_DATE=\\"$${DETECTED_BUILD_DATE})\\"
    message(windows build date is $${DETECTED_BUILD_DATE})

    GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)
    message(windows git is $${GIT_VERSION})
}

contains( COMPILE_HOST_NAME, Android ) {
    COMPILE_HOST_OS = Android
    message(Compiling with Android as host has not been tested)
}

contains( COMPILE_HOST_NAME, Linux) {
    COMPILE_HOST_OS = Linux

    DETECTED_BUILD_DATE=$$system(date)
#    DEFINES += BUILD_DATE=$$system(date)
    message(linux build date is $${DETECTED_BUILD_DATE})

    GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)
    message(linux git is $${GIT_VERSION})
 }

#message(copy keyword is $${COPY_KEYWORD})
#message(host os is $${COMPILE_HOST_OS})

android:{
#    contains(QMAKE_HOST.os,Windows){
#        #a fix for nasty bug QTBUG-69255 still not fixed in 5.12.2
#        PRO_DIR_WIN = $${_PRO_FILE_PWD_}
#        PRO_DIR_WIN ~= s,/,\\,g # replace / with \
#        QMAKE_MOVE = $${PRO_DIR_WIN}\qt_fix_move.bat
#    }

    DEFINES += TARGET_OS_ANDROID
    DEFINES += TARGET_POSIX
    ANDROID_ARM64 = 0
    ANDROID_ARMv7 = 0
    ANDROID_x86 = 0
    ANDROID_x86_64 = 0

    DEST_EXE_DIR = $$PWD/bin-Android/lib
    DEST_PYTHON_EXE_DIR = $$PWD/bin-Android/lib
    DEST_PYTHON_DLL_DIR = $$PWD/bin-Android/assets/python2.7/lib/python2.7
    DEST_PYTHON_LIB_DIR = $$PWD/bin-Android/assets/python2.7/lib/python2.7

    message(ANDROID_TARGET $${ANDROID_TARGET_ARCH})

    contains(ANDROID_TARGET_ARCH,armeabi-v8a) {
        DEFINES += TARGET_CPU_64BIT
        DEFINES += TARGET_CPU_ARM
        ANDROID_ARM64 = 1
        TARGET_ARCH_NAME=armeabi-v8a
        TARGET_CPU_BITS=64
        TARGET_ENDIAN_LITTLE=1
        TARGET_ENDIAN_BIG=0
    }

    contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        DEFINES += TARGET_CPU_64BIT
        DEFINES += TARGET_CPU_ARM
        ANDROID_ARM64 = 1
        TARGET_ARCH_NAME=armeabi-v8a
        TARGET_CPU_BITS=64
        TARGET_ENDIAN_LITTLE=1
        TARGET_ENDIAN_BIG=0
    }

    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        DEFINES += TARGET_CPU_32BIT
        DEFINES += TARGET_CPU_ARM
        ANDROID_ARMv7 = 1
        TARGET_ARCH_NAME=armeabi-v7a
        TARGET_CPU_BITS=32
        TARGET_ENDIAN_LITTLE=1
        TARGET_ENDIAN_BIG=0
    }

    contains(ANDROID_TARGET_ARCH,x86) {
        DEFINES += TARGET_CPU_32BIT
        DEFINES += TARGET_CPU_X86
        ANDROID_x86 = 1
        TARGET_ARCH_NAME=x86
        TARGET_CPU_BITS=32
        TARGET_ENDIAN_LITTLE=1
        TARGET_ENDIAN_BIG=0
    }


    contains(ANDROID_TARGET_ARCH,x86_64) {
        DEFINES += TARGET_CPU_64BIT
        DEFINES += TARGET_CPU_X86_64
        ANDROID_x86_64 = 1
        TARGET_ARCH_NAME=x86_64
        TARGET_CPU_BITS=64
        TARGET_ENDIAN_LITTLE=1
        TARGET_ENDIAN_BIG=0
    }

    DEST_SHARED_LIBS_DIR = $$PWD/bin-Android/libs/$${TARGET_ARCH_NAME}
    DEST_EXE_DIR = $$PWD/bin-Android/libs/$${TARGET_ARCH_NAME}
    DEST_PYTHON_EXE_DIR = $$PWD/bin-Android/libs/$${TARGET_ARCH_NAME}
    DEST_PYTHON_DLL_DIR = $$PWD/bin-Android/assets/kodi/python2.7/lib/python2.7
    DEST_PYTHON_LIB_DIR = $$PWD/bin-Android/assets/kodi/python2.7/lib/python2.7

#    contains(QMAKE_HOST.os,Windows):{
#        DEST_EXE_DIR ~= s,/,\\,g # replace / with \
#        DEST_SHARED_LIBS_DIR ~= s,/,\\,g # replace / with \
#        DEST_PYTHON_EXE_DIR ~= s,/,\\,g # replace / with \
#        DEST_PYTHON_DLL_DIR ~= s,/,\\,g # replace / with \
#        DEST_PYTHON_LIB_DIR ~= s,/,\\,g # replace / with \
#    }
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

    DEST_SHARED_LIBS_DIR = $$PWD/bin-Windows/
    DEST_EXE_DIR = $$PWD/bin-Windows/
    DEST_PYTHON_EXE_DIR = $$PWD/bin-Windows/
    DEST_PYTHON_DLL_DIR = $$PWD/bin-Windows/assets/kodi/system/Python/DLLs/
    DEST_PYTHON_LIB_DIR = $$PWD/bin-Windows/assets/kodi/system/Python/Lib/
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

    DEST_SHARED_LIBS_DIR = $$PWD/bin-Linux/
    DEST_EXE_DIR = $$PWD/bin-Linux/
    DEST_PYTHON_EXE_DIR = $$PWD/bin-Linux/
    DEST_PYTHON_DLL_DIR = $$PWD/bin-Linux/assets/kodi/system/Python/DLLs/
    DEST_PYTHON_LIB_DIR = $$PWD/bin-Linux/assets/kodi/system/Python/Lib/
}

macx{
    DEFINES += TARGET_OS_APPLE
    DEFINES += TARGET_POSIX
    DEFINES += TARGET_CPU_64BIT
    TARGET_CPU_BITS=64
    TARGET_ENDIAN_LITTLE=0
    TARGET_ENDIAN_BIG=1
}

win32:TARGET_OS_NAME = Windows
unix:!android: TARGET_OS_NAME = Linux
android: TARGET_OS_NAME = Android
macx: TARGET_OS_NAME = Apple

CONFIG(debug, debug|release){
    BUILD_TYPE=Debug
}

CONFIG(release, debug|release){
    BUILD_TYPE=Release
}

#message(host os is $${HOST_OS} target os is $${TARGET_OS_NAME})

#contains( TARGET_CPU_BITS, 64 ) {
#    message(Building $${TARGET_OS_NAME} $${BUILD_TYPE} 64 bit $${TARGET_ARCH_NAME} )
#}

#contains( TARGET_CPU_BITS, 32 ) {
#    message(Building $${TARGET_OS_NAME} $${BUILD_TYPE} 32 bit $${TARGET_ARCH_NAME}  )
#}

DEST_STATIC_LIBS_DIR=$$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/
SHARED_LIB_BUILD_DIR=$$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/
