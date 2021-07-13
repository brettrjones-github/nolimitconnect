
message(STATUS "HOST OS ${CMAKE_HOST_SYSTEM}")
message(STATUS "COMPILER ${CMAKE_CXX_COMPILER_ID}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(TARGET_OS_WINDOWS 1)
elseif(${CMAKE_HOST_SYSTEM} STREQUAL "Windows")
    set(TARGET_OS_ANDROID 1)
else()
    set(TARGET_OS_LINUX 1)
endif()


set(MODULE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRC_TREE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)
message(STATUS "List Dir           : ${CMAKE_CURRENT_LIST_DIR}")

if(TARGET_OS_WINDOWS)
    set(PLATFORM_MODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../platform/Windows/)
    set(BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../bin-Windows/)
    set(STATIC_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-staticlibs/Windows/x64/)
    set(SHARED_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-sharedlibs/Windows/x64/)
    message(STATUS "TARGET_OS_WINDOWS=ON")
    message(STATUS "Windows Bin Dir   : ${BIN_DIR}")

elseif(TARGET_OS_ANDROID)
    set(PLATFORM_MODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../platform/Android/)
    set(BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../bin-Android/)
    set(STATIC_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-staticlibs/Android/arm/)
    set(SHARED_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-sharedlibs/Android/arm/)
    message(STATUS "TARGET_OS_ANDROID=ON")
    message(STATUS "Android Bin Dir           : ${BIN_DIR}")

elseif(TARGET_OS_LINUX)
    set(PLATFORM_MODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../platform/Linux/)
    set(BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../bin-Linux/)
    set(STATIC_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-staticlibs/Linux/x64/)
    set(SHARED_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-sharedlibs/Linux/x64/)
    message(STATUS "TARGET_OS_LINUX=ON")
    message(STATUS "Linux Bin Dir           : ${BIN_DIR}")

else()
    message(STATUS "*** NO OS PLATFORM HAS BEEN DEFINED.. defaulting to windows")

    set(PLATFORM_MODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../platform/Windows/)
    set(BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../bin-Windows/)
    set(STATIC_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-staticlibs/Windows/x64/)
    set(SHARED_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-sharedlibs/Windows/x64/)
endif(TARGET_OS_WINDOWS)

list(APPEND CMAKE_MODULE_PATH ${PLATFORM_MODULE_DIR})

message(STATUS "Bin Dir           : ${BIN_DIR}")
