cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
include(ConfigQt)

project(SimpleCMakeApp
    DESCRIPTION "Simple Test App"
    HOMEPAGE_URL "https://www.nolimitconnet.org/"
    LANGUAGES CXX
)

# use CONFIG mode first in find_package
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
# version requirements
set(minQtVersion 5.12)

message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
include(ConfigQt)

project(SimpleCMakeApp
    DESCRIPTION "Simple Test App"
    HOMEPAGE_URL "https://www.nolimitconnet.org/"
    LANGUAGES CXX C 
)

# use CONFIG mode first in find_package
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
# version requirements
set(minQtVersion 5.12)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/DependLibs)

