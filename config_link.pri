

#link dependent library

#link dependent libraries
CONFIG(debug, debug|release){
# message(Link in DEBUG mode.)
 android{
  STATIC_LIB_PREFIX=$$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/lib
  STATIC_LIB_SUFFIX=AndroidD.a

  SHARED_LIB_BIN_DIR=$$PWD/bin-$${TARGET_OS_NAME}/libs/$${TARGET_ARCH_NAME}/
  SHARED_LIB_PREFIX=$$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/lib
  SHARED_LIB_SUFFIX=Android.so #leave out the _d for android python
  SHARED_PYTHON_LIB_SUFFIX=.so #leave out the _d for android python
  SHARED_SSL_LIB_SUFFIX=_$${TARGET_ARCH_NAME}.so #leave out the _d for android python
  PYTHON_EXTENTION_LIB_SUFFIX=_d.pyd
 }

 unix:!android{
  STATIC_LIB_PREFIX=$$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/lib
  STATIC_LIB_SUFFIX=LinuxD.a

  SHARED_LIB_BIN_DIR=$$PWD/bin-$${TARGET_OS_NAME}/
  SHARED_LIB_PREFIX=$$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/lib
  SHARED_LIB_SUFFIX=Linux_d.so
  SHARED_PYTHON_LIB_SUFFIX=_d.so
  SHARED_SSL_LIB_SUFFIX=_d.so
  PYTHON_EXTENTION_LIB_SUFFIX=_d.pyd
 }
}

CONFIG(release, debug|release){
# message(Link in RELEASE mode.)
    android:{
        STATIC_LIB_PREFIX=$$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/lib
        STATIC_LIB_SUFFIX=Android.a

        SHARED_LIB_BIN_DIR=$$PWD/bin-$${TARGET_OS_NAME}/libs/$${TARGET_ARCH_NAME}/
        SHARED_LIB_BUILD_DIR=$$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/
        SHARED_LIB_PREFIX=$$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/lib
        SHARED_LIB_SUFFIX=Android.so
        SHARED_PYTHON_LIB_SUFFIX=.so
        SHARED_SSL_LIB_SUFFIX=_$${TARGET_ARCH_NAME}.so
        PYTHON_EXTENTION_LIB_SUFFIX=.pyd
    }

    unix:!android:{
        STATIC_LIB_PREFIX=$$PWD/build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/lib
        STATIC_LIB_SUFFIX=Linux.a

        SHARED_LIB_BIN_DIR=$$PWD/bin-$${TARGET_OS_NAME}/
        SHARED_LIB_PREFIX=$$PWD/build-sharedlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/lib
        SHARED_LIB_SUFFIX=Linux.so
        SHARED_PYTHON_LIB_SUFFIX=.so
        SHARED_SSL_LIB_SUFFIX=.so
        PYTHON_EXTENTION_LIB_SUFFIX=.pyd
    }
}

#message("**TARGET_OS_NAME-> "$${TARGET_OS_NAME})
#message("**TARGET_ARCH_NAME-> "$${TARGET_ARCH_NAME})
#message("**STATIC_LIB_PREFIX-> "$${STATIC_LIB_PREFIX})
#message("**STATIC_LIB_SUFFIX-> "$${STATIC_LIB_SUFFIX})
#message("**SHARED_LIB_PREFIX-> "$${SHARED_LIB_PREFIX})
#message("**SHARED_PYTHON_LIB_SUFFIX-> "$${SHARED_PYTHON_LIB_SUFFIX})

SHARED_LIB_APPEND = .dll

CONFIG(debug, debug|release) {
win32 {
    SHARED_LIB_APPEND =  _d.dll
}

unix: {
    SHARED_LIB_APPEND =  _d
}
}

CONFIG(release, debug|release) {
win32 {
    SHARED_LIB_APPEND =  .dll
}

unix: {
    SHARED_LIB_APPEND =
}
}

