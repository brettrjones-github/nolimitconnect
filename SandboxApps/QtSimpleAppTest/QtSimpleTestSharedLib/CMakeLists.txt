include(ConfigQt)
find_package(Qt5 COMPONENTS Widgets Test Core REQUIRED)

set(BUILD_SHARED_LIBS ON)
add_compile_definitions(MAKE_SHARED_LIB=1 TARGET_OS_WINDOWS=1)

	
set ( HEADER_FILE_ONLY
	qttestsharedlib3.h 
)

set ( SOURCES
	qttestsharedlib3.cpp
)
include(ConfigQt)
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

set ( HEADER_FILE_ONLY
	qttestsharedlib3.h
)

set ( SOURCES
	qttestsharedlib3.cpp
)

add_library("QtSimpleTestSharedLib" SHARED ${SOURCES})

#if(NOT BUILD_SHARED_LIBS)
# set(STATIC_POSTFIX "-s")
#endif()

#append library type 
#set_target_properties("QtSimpleTestSharedLib" PROPERTIES
#  DEBUG_POSTFIX "${STATIC_POSTFIX}-d"
#  RELEASE_POSTFIX "${STATIC_POSTFIX}"
#  MINSIZEREL_POSTFIX "${STATIC_POSTFIX}-mr"
#  RELWITHDEBINFO_POSTFIX "${STATIC_POSTFIX}-rd"
#)

target_link_libraries("QtSimpleTestSharedLib" Qt5::Core)

if(${WINDOWS})
#    target_sources(${PROJECT_NAME}-librw PUBLIC WindowsFileReader.cpp WindowsFileWriter.cpp)
elseif(${MAC})
#    target_sources(${PROJECT_NAME}-librw PUBLIC MacFileReader.cpp MacFileWriter.cpp)
else()
#    target_sources(${PROJECT_NAME}-librw PUBLIC FileReader.cpp FileWriter.cpp)
endif()