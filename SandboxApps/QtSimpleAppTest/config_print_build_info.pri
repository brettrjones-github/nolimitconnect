

contains( COMPILE_HOST_NAME, Linux) {
    message(Compiling with Linux as host)
}

contains( COMPILE_HOST_NAME, Android ) {
    message(Compiling with Android as host has not been tested)
}

contains( COMPILE_HOST_NAME, Windows ) {
    message(Compiling with Windows as host)
}

message(compile host os is $${COMPILE_HOST_OS} target os is $${TARGET_OS_NAME})

message(make directory cmd $${PRO_MKDIR_CMD})
message(copy cmd $${PRO_MKDIR_CMD})
message(move cmd $${PRO_RENAME_CMD})

contains( TARGET_CPU_BITS, 64 ) {
    message(Building $${BUILD_TYPE} $${TARGET_OS_NAME} 64 bit $${TARGET_ARCH_NAME} )
}

contains( TARGET_CPU_BITS, 32 ) {
    message(Building $${BUILD_TYPE} $${TARGET_OS_NAME} 32 bit $${TARGET_ARCH_NAME} )
}

message(static libs dir $${BUILD_STATIC_LIBS_DIR} shared libs dir $${BUILD_SHARED_LIB_DIR} exe dir $${DEST_EXE_DIR} )

message(Shared extension $${SHARED_LIB_APPEND} Static extension $${STATIC_LIB_APPEND} )
