

android {
    versionAtMost(QT_VERSION, 5.15.2) {
        QT += androidextras
    }

    # Default rules for deployment.
    include(config_deploy.pri)

    DISTFILES += \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/AndroidManifest.xml \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew \
        android/gradlew.bat \
        android/src/com/example/myproject/MySubclassOfQtActivity.java

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}
