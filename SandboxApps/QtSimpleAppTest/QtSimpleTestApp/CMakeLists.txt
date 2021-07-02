
include(ConfigQt)

project(QtSimpleTestApp
    DESCRIPTION "Simple Test App"
    HOMEPAGE_URL "https://www.nolimitconnet.org/"
    VERSION 1.0.0
    LANGUAGES CXX
)

#project(QtSimpleTestApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set ( SOURCES
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)


find_package(Qt5 COMPONENTS GUI Test Widgets Core Svg REQUIRED)

add_executable(QtSimpleTestApp ${SOURCES})

target_link_libraries(QtSimpleTestApp Qt5::Widgets QtSimpleTestStaticLib QtSimpleTestSharedLib )
