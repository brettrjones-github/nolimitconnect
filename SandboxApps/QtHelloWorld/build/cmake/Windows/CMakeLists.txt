cmake_minimum_required(VERSION 3.12.2 FATAL_ERROR)
project(HelloWorldApp
    DESCRIPTION "Hello World App"
    VERSION 1.0.0
    HOMEPAGE_URL "https://www.nolimitconnet.org/"
    LANGUAGES CXX
)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# use CONFIG mode first in find_package
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
# version requirements
set(minQtVersion 5.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../../cmake/modules/)

include(ConfigOs)
include(ConfigQt)
include(PrintEnv)


# find_package(Qt5 COMPONENTS GUI Test Widgets Core Svg REQUIRED)
# find must be done after project is set
find_package(Qt5Widgets REQUIRED)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})



#set( SOURCES
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../QtHelloWorld/mainwindow.ui
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../QtHelloWorld/mainwindow.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../QtHelloWorld/main.cpp
#)

include(${CMAKE_CURRENT_LIST_DIR}/../../../QtHelloWorld/CMakeLists.txt)

add_executable(HelloWorldApp ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
