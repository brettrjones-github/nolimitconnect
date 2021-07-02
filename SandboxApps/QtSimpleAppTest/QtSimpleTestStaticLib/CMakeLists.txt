include(ConfigQt)
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

set ( HEADER_FILE_ONLY
	qtteststaticlib3.h
)

set ( SOURCES
	qtteststaticlib3.cpp
)

add_library("QtSimpleTestStaticLib" STATIC ${SOURCES})
target_link_libraries("QtSimpleTestStaticLib" Qt5::Core)

#if(NOT BUILD_SHARED_LIBS)
# set(STATIC_POSTFIX "-s")
#endif()

#append library type 
#set_target_properties("QtSimpleTestStaticLib" PROPERTIES
#  DEBUG_POSTFIX "${STATIC_POSTFIX}-d"
#  RELEASE_POSTFIX "${STATIC_POSTFIX}"
#  MINSIZEREL_POSTFIX "${STATIC_POSTFIX}-mr"
#  RELWITHDEBINFO_POSTFIX "${STATIC_POSTFIX}-rd"
#)
