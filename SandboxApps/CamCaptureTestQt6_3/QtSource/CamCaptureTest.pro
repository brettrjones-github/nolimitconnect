TEMPLATE = app
TARGET = camcapturetest
CONFIG += no_docs_target

QT += gui core concurrent widgets network opengl xml svg
QT += multimedia
QT += multimediawidgets

android:{
    versionAtMost(QT_VERSION, 5.15.2){
        QT += androidextras
    }

    versionAtLeast(QT_VERSION, 6.2.0){
        # temporary crap while qt 6.2 is being developed
        # still broken in 6.2.1
        QT += core-private
        CONFIG += future
    }
}

INCLUDEPATH += $$PWD/../../../
INCLUDEPATH += $$PWD/../../../DependLibs

include($$PWD/../../../config_os_detect.pri)

HEADERS = \
    CamCaptureTest.h \
    CamLogic.h \
	VideoSinkGrabber.h \
	VxLabel.h \
    GuiHelpers.h \
    GuiParams.h

SOURCES = \
    main.cpp \
    CamCaptureTest.cpp \
    CamLogic.cpp \
	VideoSinkGrabber.cpp \
	VxLabel.cpp \
    GuiHelpers.cpp \
    GuiParams.cpp

FORMS += \
    CamCaptureTest.ui



PRE_TARGETDEPS +=  $$PWD/../../../build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/$${STATIC_LIB_PREFIX}CoreLib$${STATIC_LIB_SUFFIX}
PRE_TARGETDEPS +=  $$PWD/../../../build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/$${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}

LIBS +=  $$PWD/../../../build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/$${STATIC_LIB_PREFIX}CoreLib$${STATIC_LIB_SUFFIX}
LIBS +=  $$PWD/../../../build-staticlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}/$${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
OTHER_FILES += android/AndroidManifest.xml

DISTFILES += \
     $$PWD/android/build.gradle \
     $$PWD/android/gradle.properties \
     $$PWD/android/gradle/wrapper/gradle-wrapper.jar \
     $$PWD/android/gradle/wrapper/gradle-wrapper.properties \
     $$PWD/android/gradlew \
     $$PWD/android/gradlew.bat \
     $$PWD/android/res/values/libs.xml
