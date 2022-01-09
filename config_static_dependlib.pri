
# include(config_version.pri) # not needed unless building kodi
include(config_os_detect.pri)
include(config_compiler.pri)
include(config_opensslp_include.pri)

QT       -= gui
CONFIG += staticlib
CONFIG -= sharedlib

DESTDIR = $${DEST_STATIC_LIBS_DIR}

DEFINES += LIB_STATIC_LIB


TARGET_LIB_APPEND = .lib

CONFIG(debug, debug|release) {
win32 {
    TARGET_LIB_APPEND =  D.lib
}

unix {
    TARGET_LIB_APPEND =  D
}

}

CONFIG(release, debug|release) {
win32 {
    TARGET_LIB_APPEND =  .lib
}

unix {
    TARGET_LIB_APPEND =
}

}

TARGET=$${TARGET_NAME}$${TARGET_OS_NAME}$${TARGET_LIB_APPEND}

OBJECTS_DIR=.objs/$${TARGET_NAME}/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}


