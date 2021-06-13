
TEMPLATE = lib
CONFIG -= staticlib
CONFIG += sharedlib
CONFIG += no_docs_target
CONFIG += c++11

DEFINES += MAKE_SHARED_LIB

QT       += core gui svg widgets

TARGET = QtSimpleTestSharedLib
TARGET_NAME=QtSimpleTestSharedLib

include(../config_detect_os.pri)

SOURCES += \
        $$PWD/qttestsharedlib3.cpp

HEADERS += \
        $$PWD/qttestsharedlib3.h

message(QtSimpleTestSharedLib OUT_PWD $$OUT_PWD)

#BRJ copy commands do not work on windows
#copydata.commands = $${PRO_COPY_CMD} $$OUT_PWD/lib$${TARGET_NAME}$${SHARED_LIB_APPEND} $${BUILD_SHARED_LIB_DIR}
#copydata.commands = $$shell_quote($${PRO_COPY_CMD} J:/SandboxApps/build-QtSimpleTestProject-Android_Qt_5_15_0_Clang_Multi_Abi-Debug/QtSimpleTestSharedLib/libQtSimpleTestSharedLib_armeabi-v7a.so J:/#SandboxApps/QtSimpleAppTest/build-sharedlibs/Android/armeabi-v7a/Debug)
#first.depends = $(first) copydata
#export(first.depends)
#export(copydata.commands)
#QMAKE_EXTRA_TARGETS += shell_quote($${PRO_COPY_CMD} J:/SandboxApps/build-QtSimpleTestProject-Android_Qt_5_15_0_Clang_Multi_Abi-Debug/QtSimpleTestSharedLib/libQtSimpleTestSharedLib_armeabi-v7a.so J:/SandboxApps/QtSimpleAppTest/build-sharedlibs/Android/armeabi-v7a/Debug)
#copydata.commands = $${PRO_COPY_CMD} $$OUT_PWD/lib$${TARGET_NAME}$${SHARED_LIB_APPEND} $${BUILD_SHARED_LIB_DIR}
#copydata.commands = $$shell_quote($${PRO_COPY_CMD} J:/SandboxApps/build-QtSimpleTestProject-Android_Qt_5_15_0_Clang_Multi_Abi-Debug/QtSimpleTestSharedLib/libQtSimpleTestSharedLib_armeabi-v7a.so J:/SandboxApps/QtSimpleAppTest/build-sharedlibs/Android/armeabi-v7a/Debug)


#BRJ this actually works for copy file after compile
#android: {
#    COPY_SIMPLE_SHARED_LIB_PARAMS=$$OUT_PWD/lib$${TARGET_NAME}$${SHARED_LIB_APPEND} $${BUILD_SHARED_LIB_DIR}/lib$${TARGET_NAME}$${SHARED_LIB_APPEND}
#    contains(QMAKE_HOST.os,Windows):{
#        COPY_SIMPLE_SHARED_LIB_PARAMS ~= s,/,\\,g # replace / with \
#    }

#    message(QtSimpleTestSharedLib copy command $${PRO_COPY_CMD} $${COPY_SIMPLE_SHARED_LIB})

#    copyfiles.commands = $(COPY_FILE) $${COPY_SIMPLE_SHARED_LIB_PARAMS}
#    QMAKE_EXTRA_TARGETS += copyfiles
#    POST_TARGETDEPS += copyfiles
#}

android: {
    DESTDIR=$${BUILD_SHARED_LIB_DIR}
}

!android: {
    # put the dll in the bin folder
    DESTDIR=$${DEST_EXE_DIR}
}
