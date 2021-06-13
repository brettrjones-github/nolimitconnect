

## create any directories that do not exist
MK_BUILD_STATIC_LIBS_DIR = $${PRO_MKDIR_CMD} $${BUILD_STATIC_LIBS_DIR}
MK_BUILD_SHARED_LIB_DIR = $${PRO_MKDIR_CMD} $${BUILD_SHARED_LIB_DIR}

MK_EXE_DIR = $${PRO_MKDIR_CMD} $${DEST_EXE_DIR}
MK_DEST_SHARED_LIBS_DIR = $${PRO_MKDIR_CMD} $${DEST_SHARED_LIBS_DIR}

MK_DEST_PYTHON_EXE_DIR = $${PRO_MKDIR_CMD} $${DEST_PYTHON_EXE_DIR}
MK_DEST_PYTHON_DLL_DIR = $${PRO_MKDIR_CMD} $${DEST_PYTHON_DLL_DIR}
MK_DEST_PYTHON_LIB_DIR = $${PRO_MKDIR_CMD} $${DEST_PYTHON_LIB_DIR}

contains(QMAKE_HOST.os,Windows):{
    MK_BUILD_STATIC_LIBS_DIR ~= s,/,\\,g # replace / with \
    MK_BUILD_SHARED_LIB_DIR ~= s,/,\\,g # replace / with \

    MK_EXE_DIR ~= s,/,\\,g # replace / with \
    MK_DEST_SHARED_LIBS_DIR ~= s,/,\\,g # replace / with \

    MK_DEST_PYTHON_EXE_DIR ~= s,/,\\,g # replace / with \
    MK_DEST_PYTHON_DLL_DIR ~= s,/,\\,g # replace / with \
    MK_DEST_PYTHON_LIB_DIR ~= s,/,\\,g # replace / with \
}

## execute make directories
system($${MK_BUILD_STATIC_LIBS_DIR})
system($${MK_BUILD_SHARED_LIB_DIR})

system($${MK_EXE_DIR})
system($${MK_DEST_SHARED_LIBS_DIR})

system($${MK_DEST_PYTHON_EXE_DIR})
system($${MK_DEST_PYTHON_DLL_DIR})
system($${MK_DEST_PYTHON_DLL_DIR})


