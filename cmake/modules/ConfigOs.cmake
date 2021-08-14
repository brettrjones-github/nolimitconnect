
message(STATUS "HOST OS          : ${CMAKE_HOST_SYSTEM}")
message(STATUS "COMPILER         : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_HOST_WIN32 : ${CMAKE_HOST_WIN32}")

if(DEFINED ANDROID_NATIVE_API_LEVEL)
	 set(TARGET_OS_ANDROID 1)
elseif(CMAKE_HOST_WIN32)
    set(TARGET_OS_WINDOWS 1)
else()
    set(TARGET_OS_LINUX 1)
endif()

set(MODULE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRC_TREE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)

if(TARGET_OS_WINDOWS)
    set(PLATFORM_MODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../platform/Windows/)
    set(BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../bin-Windows/)
    set(STATIC_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-staticlibs/Windows/x64/)
    set(SHARED_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-sharedlibs/Windows/x64/)
    set(WIN64 1)
    set(TARGET_WINDOWS 1)
    message(STATUS "TARGET_OS_WINDOWS=1")
    message(STATUS "Building ${PROJECT_NAME} for windows on windows")

elseif(TARGET_OS_ANDROID)
    set(PLATFORM_MODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../platform/Android/)
    set(BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../bin-Android/)
    set(STATIC_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-staticlibs/Android/arm/)
    set(SHARED_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-sharedlibs/Android/arm/)
    message(STATUS "TARGET_OS_ANDROID=1")
    message(STATUS "Building ${PROJECT_NAME} for android on windows")

elseif(TARGET_OS_LINUX)
    set(PLATFORM_MODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../platform/Linux/)
    set(BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../bin-Linux/)
    set(STATIC_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-staticlibs/Linux/x64/)
    set(SHARED_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-sharedlibs/Linux/x64/)
    message(STATUS "TARGET_OS_LINUX=1")
    message(STATUS "Building ${PROJECT_NAME} for linux on windows")

else()
    message(STATUS "*** NO OS PLATFORM HAS BEEN DEFINED.. defaulting to windows")

    set(PLATFORM_MODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../platform/Windows/)
    set(BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../bin-Windows/)
    set(STATIC_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-staticlibs/Windows/x64/)
    set(SHARED_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../build-sharedlibs/Windows/x64/)
endif(TARGET_OS_WINDOWS)

list(APPEND CMAKE_MODULE_PATH ${PLATFORM_MODULE_DIR})

message(STATUS "Bin Dir           : ${BIN_DIR}")
