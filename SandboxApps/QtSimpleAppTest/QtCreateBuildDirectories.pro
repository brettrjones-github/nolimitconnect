
TEMPLATE     = lib

CONFIG += no_docs_target
versionAtMost(QT_VERSION, 5.15.2){
    CONFIG += c++11
}

versionAtLeast(QT_VERSION, 6.0.0){
    CONFIG += c++17
}

#define this next line to make desired android build directories of multiple build types
ANDROID_ABIS=armeabi-v7a armeabi-v8a x86_64

include(config_detect_os.pri)
include(config_create_build_directories.pri)
include(config_print_build_info.pri)

