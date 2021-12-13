
QT       += core gui widgets svg

TARGET = NetClientTest
TEMPLATE = app

versionAtMost(QT_VERSION, 5.15.2){
    CONFIG += c++11
}

versionAtLeast(QT_VERSION, 6.0.0){
    CONFIG += c++17
}

INCLUDEPATH += $$PWD/NetClientTest

INCLUDEPATH += $$PWD/../../DependLibs
INCLUDEPATH += $$PWD/../../


SOURCES += \
        $$PWD/NetClientTest/main.cpp \
        $$PWD/NetClientTest/mainwindow.cpp \
		$$PWD/NetClientTest/AppLogic.cpp \
		$$PWD/NetClientTest/AppSettings.cpp \
		$$PWD/NetClientTest/LogWidget.cpp \
		$$PWD/NetClientTest/NetTestUtil.cpp \
		$$PWD/NetClientTest/NetWidget.cpp \
		$$PWD/NetClientTest/NlcPingResponseServer.cpp \
		$$PWD/NetClientTest/NlcTestThread.cpp

HEADERS += \
        $$PWD/NetClientTest/mainwindow.h \
		$$PWD/NetClientTest/AppLogic.h \
		$$PWD/NetClientTest/AppSettings.h \
		$$PWD/NetClientTest/LogWidget.h \
		$$PWD/NetClientTest/NetTestUtil.h \
		$$PWD/NetClientTest/NetWidget.h \
		$$PWD/NetClientTest/NlcPingResponseServer.h \
		$$PWD/NetClientTest/NlcTestCommon.h \
		$$PWD/NetClientTest/NlcTestThread.h

FORMS += \
        $$PWD/NetClientTest/mainwindow.ui \
		$$PWD/NetClientTest/LogWidget.ui \
        $$PWD/NetClientTest/NetWidget.ui



android:{
CONFIG += mobility
MOBILITY =
    DEFINES +=__ANDROID_API__=21
    DEFINES += BIONIC_IOCTL_NO_SIGNEDNESS_OVERLOAD

#SHARED_LIB_RES_DIR = $$PWD/androidsimpletest/libs/armeabi-v7a/

ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/androidnetclienttest/


INCLUDEPATH += $$PWD/DependLibs

CONFIG(release, debug|release){
    DEFINES += NDEBUG
    DEFINES += RELEASE
    DEFINES += QT_NO_DEBUG_OUTPUT

    LIBS += -l$$PWD/build-staticlibs/Android/armeabi-v7a/Release/libnetlibAndroid.a
    LIBS += -l$$PWD/build-staticlibs/Android/armeabi-v7a/Release/libpktlibAndroid.a
    LIBS += -l$$PWD/build-staticlibs/Android/armeabi-v7a/Release/libcorelibAndroid.a
    LIBS += -l$$PWD/build-staticlibs/Android/armeabi-v7a/Release/libcrossguidAndroid.a
}

CONFIG(debug, debug|release){
# message(Link in DEBUG mode.)

    #if do not turn off optimization then android NDK std::string values will get optimized out and not viewable
    # sigh if optimization is turned off then the debugger gets hung up so may have to live with not being able to view strings.. also ndk strings sometimes are still optimized out
    QMAKE_CXXFLAGS -= -O1
    QMAKE_CXXFLAGS -= -O2
    QMAKE_CXXFLAGS -= -O3
    QMAKE_CXXFLAGS -= -O4
    QMAKE_CXXFLAGS += -O0

    #message(cxx flasgs $${QMAKE_CXXFLAGS})

    LIBS += -l$$PWD/../../build-staticlibs/Android/armeabi-v7a/Debug/libnetlibAndroidD.a
    LIBS += -l$$PWD/../../build-staticlibs/Android/armeabi-v7a/Debug/libpktlibAndroidD.a
    LIBS += -l$$PWD/../../build-staticlibs/Android/armeabi-v7a/Debug/libcorelibAndroidD.a
    LIBS += -l$$PWD/../../build-staticlibs/Android/armeabi-v7a/Debug/libcrossguidAndroidD.a
}

#static libs


#os libs
LIBS +=  -ldl -lm -landroid -lc -lstdc++ -llog

contains( COMPILE_HOST_NAME, Linux) {
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
} # end android config
}

message($$[QT_INSTALL_BINS])
