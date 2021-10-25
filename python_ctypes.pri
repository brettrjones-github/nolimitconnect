

INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/include
INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi


HEADERS += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/include/ffi.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/include/ffi_common.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/fficonfig.h


contains(TARGET_ARCH_NAME,x86) {
# message(Python ctypes x86)

INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/x86

HEADERS += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/x86/ffitarget.h
}

contains(TARGET_ARCH_NAME,x86_64) {
# message(Python ctypes x86_64)

INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/x86

HEADERS += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/x86/ffitarget.h
}

contains( TARGET_ARCH_NAME, armeabi-v7a ) {
# message(Python ctypes arm)

INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/arm

HEADERS += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/arm/ffitarget.h
}

contains( TARGET_ARCH_NAME, arm64-v8a ) {
# message(Python ctypes arm64)

INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/aarch64

HEADERS += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/aarch64/ffitarget.h
}

win32{
INCLUDEPATH += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi_msvc


HEADERS += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/ctypes.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/ctypes_dlfcn.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi_msvc/ffi.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi_msvc/ffi_common.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi_msvc/fficonfig.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi_msvc/ffitarget.h

SOURCES += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/_ctypes.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/callbacks.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/callproc.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/cfield.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi_msvc/ffi.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi_msvc/prep_cif.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi_msvc/win32.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/malloc_closure.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/stgdict.c
}

android:{
INCLUDEPATH += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi

HEADERS += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/ctypes.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/ctypes_dlfcn.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/include/ffi.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/include/ffi_common.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/arm/ffitarget.h \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/fficonfig.h

contains( TARGET_ARCH_NAME, armeabi-v7a ) {
SOURCES += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/_ctypes.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/callbacks.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/callproc.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/cfield.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/malloc_closure.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/arm/ffi_arm.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/arm/sysv.S \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/arm/trampoline.S \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/stgdict.c
    }

contains( TARGET_ARCH_NAME, arm64-v8a ) {
SOURCES += 	$$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/_ctypes.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/callbacks.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/callproc.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/cfield.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/malloc_closure.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/aarch64/ffi.c \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/aarch64/sysv.S \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/stgdict.c
    }
}

SOURCES += \
    $$PWD/DependLibs/Python-2.7.14/Modules/_ctypes/libffi/src/prep_cif.c
