# Enable C++17
set(minQtVersion 6.1.2)
set(CMAKE_CXX_STANDARD 17) # qt 6.1.2 requires c++ standard 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find must be done after project is set
find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Svg)
find_package(Qt6 COMPONENTS Widgets)
find_package(Qt6 COMPONENTS OpenGL)
find_package(Qt6 COMPONENTS OpenGLWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${Qt6Core_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_LIST_DIR}/QtSource/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../GuiInterface/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../ConfigHeaders/)


set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_LIST_DIR}/QtSource/AccountMgr.h
   )

set(SOURCES
   ${SOURCES}
   ${CMAKE_CURRENT_LIST_DIR}/QtSource/AccountMgr.cpp
   )
