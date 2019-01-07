
DEFINES += LIB_STATIC _LIB

CONFIG += staticlib qt thread silent

TARGET_LIB_APPEND = .lib

CONFIG(debug, debug|release) {
    DESTDIR = ../../debug
win32 {
    TARGET_LIB_APPEND =  D.lib
}

unix: {
    TARGET_LIB_APPEND =  D
}

}

CONFIG(release, debug|release) {
    DESTDIR = ../../release
win32 {
    TARGET_LIB_APPEND =  .lib
}

unix: {
    TARGET_LIB_APPEND =
}

}

INCLUDEPATH += ./../../
INCLUDEPATH += ./../../DependLibs
