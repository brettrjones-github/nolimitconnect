

set(LIB_NAME "QtSimpleTestStaticLib")
# Enable C++17
set(CMAKE_CXX_STANDARD 17)

include_directories(${Qt6Core_INCLUDE_DIRS})

set(HEADERS
    ${HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/qtteststaticlib3.h
)

set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/qtteststaticlib3.cpp
)


#target_include_directories(staticLibA INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
add_library(${LIB_NAME} STATIC ${SOURCES})


