
set(LIB_NAME "QtSimpleTestSharedLib")
# Enable C++17
SET(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)

include_directories(${Qt6Core_INCLUDE_DIRS})

set(SOURCES
   ${CMAKE_CURRENT_LIST_DIR}/qttestsharedlib3.cpp
)
set(HEADERS
   ${CMAKE_CURRENT_LIST_DIR}/qttestsharedlib3.h 
)



add_library(${LIB_NAME} SHARED ${SOURCES})
