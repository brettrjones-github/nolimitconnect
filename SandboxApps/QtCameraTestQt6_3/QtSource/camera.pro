TEMPLATE = app
TARGET = camera

QT += multimedia multimediawidgets

HEADERS = \
    camera.h \
    imagesettings.h \
    videosettings.h \
    VideoSinkGrabber.h \
    metadatadialog.h

SOURCES = \
    main.cpp \
    camera.cpp \
    imagesettings.cpp \
    videosettings.cpp \
    VideoSinkGrabber.cpp \
    metadatadialog.cpp

FORMS += \
    imagesettings.ui

android|ios {
    FORMS += \
        camera_mobile.ui \
        videosettings_mobile.ui
} else {
    FORMS += \
        camera.ui \
        videosettings.ui
}
RESOURCES += camera.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/camera
INSTALLS += target

QT += widgets
include(../../multimedia/shared/shared.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
OTHER_FILES += android/AndroidManifest.xml

DISTFILES += \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml
