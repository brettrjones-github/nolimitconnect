

set(LIB_NAME "QtSimpleTestStaticLib")
# Enable C++17
SET(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)

include_directories(${Qt6Core_INCLUDE_DIRS})


set(SOURCES
   ${CMAKE_CURRENT_LIST_DIR}/qtteststaticlib3.cpp
)

set(HEADERS
   ${CMAKE_CURRENT_LIST_DIR}/qtteststaticlib3.h 
)

#target_include_directories(staticLibA INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
add_library(${LIB_NAME} STATIC ${SOURCES})


