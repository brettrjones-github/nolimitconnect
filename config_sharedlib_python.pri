
include(config_sharedlib_defines.pri)

unix:!android{
    QMAKE_CXXFLAGS += -fPIC
    QMAKE_CFLAGS += -fPIC

    DESTDIR = $$PWD/build-pythonlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
}

android{
    QMAKE_CXXFLAGS += -fPIC
    QMAKE_CFLAGS += -fPIC
    #part of workaround for QTBUG-69255 .. do not define DESTDIR because uses $$PWD_OUT as root of path
    !contains(COMPILE_HOST_OS,Windows){
        DESTDIR = $$PWD/build-pythonlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}
    }
}

#do not append os name to name so can have standard python name
TARGET=$${TARGET_NAME}$${SHARED_LIB_APPEND}

INCLUDEPATH += $PWD/DependLibs/Python-2.7.14
INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14/Include


