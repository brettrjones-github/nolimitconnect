cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE on)

set(PROJECT_NAME "HelloWorldApp")

project(${PROJECT_NAME}
    DESCRIPTION "Hello World App"
    VERSION 1.0.0
    HOMEPAGE_URL "https://www.nolimitconnet.org/"
    LANGUAGES CXX
)

if(NOT DEFINED INSTALL_PROJECT_DIR)
  set(INSTALL_PROJECT_DIR "nlc-examples")
endif()

set(minQtVersion 6.1.2)
# find must be done after project is set
find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Widgets)
find_package(Qt6 COMPONENTS OpenGL)
find_package(Qt6 COMPONENTS OpenGLWidgets)

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# use CONFIG mode first in find_package
#set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
# version requirements

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../cmake/modules/)

include(ConfigOs)
#include(ConfigQt)
include(PrintEnv)

if(TARGET_OS_ANDROID)
    set(QT_ANDROID_MANIFEST_TEMPLATE ${CMAKE_CURRENT_LIST_DIR}/../../../../android_qt_manifest/QtHellowWorld/android/AndroidManifest.xml)
    # store the current source directory for future use
    #set(CMAKE_INSTALL_PREFIX ${BIN_DIR})
    set(QT_ANDROID_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../QtHelloWorld/)
    #set(QT_ANDROID_APP_BINARY_DIR ${BIN_DIR})
    #set(QT_ANDROID_BUILD_TYPE "Debug")
    #set(TARGET_LEVEL_OPTIONS "Debug")
#    set(ANDROID_NATIVE_API_LEVEL android-21)
#    set(ANDROID_PLATFORM android-21)
#    set(ANDROID_ABIS "armeabi-v7a")
#    set(SUPPORTED_ABIS "armeabi-v7a")
#    set(Build.ANDROID_ABIS "armeabi-v7a")
#    set(Build.SUPPORTED_ABIS "armeabi-v7a")
#    set(ANDROID_BUILD_ABI_armeabi-v7a ON)
#    set(ANDROID_ABI "armeabi-v7a")
#    set(ANDROID_TOOLCHAIN_NAME "^arm-linux-androideabi-armeabi-v7a")


    #include(AddQtAndroidApk)

    # set(QT_ANDROID_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../QtHelloWorld/)
    # set(CMAKE_CURRENT_BINARY_DIR ${BIN_DIR})
    message(STATUS "QT_ANDROID_MANIFEST_TEMPLATE ${QT_ANDROID_MANIFEST_TEMPLATE}")
    message(STATUS "QT_ANDROID_SOURCE_DIR ${QT_ANDROID_SOURCE_DIR}")
    message(STATUS "CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "QT_ANDROID_APP_BINARY_DIR ${QT_ANDROID_APP_BINARY_DIR}")
    message(STATUS "CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")
    message(STATUS "QT_ANDROID_BUILD_TYPE ${QT_ANDROID_BUILD_TYPE}")
    message(STATUS "TARGET_LEVEL_OPTIONS ${TARGET_LEVEL_OPTIONS}")
    message(STATUS "INSTALL_OPTIONS ${INSTALL_OPTIONS}")
    message(STATUS "SIGN_OPTIONS ${SIGN_OPTIONS}")
    message(STATUS "ANDROID_NATIVE_API_LEVEL ${ANDROID_NATIVE_API_LEVEL}")
    message(STATUS "ANDROID_ABI ${ANDROID_ABI}")
    message(STATUS "CMAKE_ANDROID_NDK  ${CMAKE_ANDROID_NDK}")
    message(STATUS "CMAKE_ANDROID_STL_TYPE   ${CMAKE_ANDROID_STL_TYPE}")
    message(STATUS "CMAKE_SYSTEM_NAME   ${CMAKE_SYSTEM_NAME}")
    message(STATUS "CMAKE_SYSTEM_VERSION   ${CMAKE_SYSTEM_VERSION}")
    message(STATUS "CMAKE_ANDROID_ARCH_ABI   ${CMAKE_ANDROID_ARCH_ABI}")

    message(STATUS "CMAKE_C_COMPILER   ${CMAKE_C_COMPILER}")
    message(STATUS "CMAKE_CXX_COMPILER   ${CMAKE_CXX_COMPILER}")
    message(STATUS "CMAKE_TOOLCHAIN_FILE   ${CMAKE_TOOLCHAIN_FILE}")
    message(STATUS "CMAKE_MAKE_PROGRAM   ${CMAKE_MAKE_PROGRAM}")
    message(STATUS "CMAKE_BUILD_TYPE   ${CMAKE_BUILD_TYPE}")
    message(STATUS "ANDROID_NATIVE_API_LEVEL   ${ANDROID_NATIVE_API_LEVEL}")
    message(STATUS "ANDROID_PLATFORM   ${ANDROID_PLATFORM}") #should be same as ANDROID_NATIVE_API_LEVEL

    message(STATUS "CMAKE_INSTALL_NAME_DIR   ${CMAKE_INSTALL_NAME_DIR}")
    message(STATUS "QT_ANDROID_QT_ROOT   ${QT_ANDROID_QT_ROOT}")
    message(STATUS "CMAKE_SOURCE_DIR   ${CMAKE_SOURCE_DIR}")

endif()

#inside c++ code use this to determine correct folder for the running abi
#String abi
#if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
#    abi = Build.SUPPORTED_ABIS[0];
#} else {
#    //noinspection deprecation
#    abi = Build.CPU_ABI;
#}
#String folder;
#if (abi.contains("armeabi-v7a")) {
#    folder = "armeabi-v7a";
#} else if (abi.contains("x86_64")) {
#    folder = "x86_64";
#} else if (abi.contains("x86")) {
#    folder = "x86";
#} else if (abi.contains("armeabi")) {
#    folder = "armeabi";
#}



# Make the executable files output to where the Android Gradle plugin expects libraries
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

#set(EXECUTABLE_OUTPUT_PATH      ${BIN_DIR})
# store the current source directory for future use

include(${CMAKE_CURRENT_LIST_DIR}/../../../QtHelloWorld/CMakeLists.txt)

qt_add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Widgets
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${INSTALL_PROJECT_DIR}"
    BUNDLE DESTINATION "${INSTALL_PROJECT_DIR}"
    LIBRARY DESTINATION "${INSTALL_PROJECT_DIR}"
)

