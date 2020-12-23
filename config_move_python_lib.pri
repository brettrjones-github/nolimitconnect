
#rename and move python extention lib to bin/Python directory
PYTHON_SRC_NAME = ""
PYTHON_SRC_DIR = ""
PYTHON_DEST_NAME = ""

PYTHON_LIB_COPY_CMD = ""

PYTHON_SRC_DIR = $$PWD/build-pythonlibs/$${TARGET_OS_NAME}/$${TARGET_ARCH_NAME}/$${BUILD_TYPE}
unix:!android{
    CONFIG(debug, debug|release){
        PYTHON_SRC_NAME = lib$${TARGET_NAME}_d.so.1.0.0
#do not use the _d extension even if debug to avoid complicated loading
#        PYTHON_DEST_NAME = $${TARGET_NAME}_d.pyd
        PYTHON_DEST_NAME = $${TARGET_NAME}.pyd
     }

     CONFIG(release, debug|release){
        PYTHON_SRC_NAME = lib$${TARGET_NAME}.so.1.0.0
        PYTHON_DEST_NAME = $${TARGET_NAME}.pyd
     }

    PYTHON_LIB_MKDIR_CMD = mkdir -p $${DEST_PYTHON_DLL_DIR}
    message("*python dll mkdir cmd $${PYTHON_LIB_MKDIR_CMD}")

    PYTHON_LIB_COPY_CMD = cp -f $${PYTHON_SRC_DIR}/$${PYTHON_SRC_NAME} $${DEST_PYTHON_DLL_DIR}
    message("**python dll cp cmd $${PYTHON_LIB_COPY_CMD}")

    PYTHON_LIB_RENAME_CMD = mv -f $${DEST_PYTHON_DLL_DIR}$${PYTHON_SRC_NAME}  $${DEST_PYTHON_DLL_DIR}$${PYTHON_DEST_NAME}
    message("***python dll rename cmd $${PYTHON_LIB_REMAME_CMD}")

    QMAKE_POST_LINK += $$quote($${PYTHON_LIB_MKDIR_CMD} && $${PYTHON_LIB_COPY_CMD} && $${PYTHON_LIB_RENAME_CMD})
    message("****python post link $${QMAKE_POST_LINK}")
}

android{
    CONFIG(debug, debug|release){
        PYTHON_SRC_NAME = lib$${TARGET_NAME}_d.so
#do not use the _d extension even if debug to avoid complicated loading
#        PYTHON_DEST_NAME = $${TARGET_NAME}_d.pyd
        PYTHON_DEST_NAME = $${TARGET_NAME}.pyd
     }

     CONFIG(release, debug|release){
        PYTHON_SRC_NAME = lib$${TARGET_NAME}.so
        PYTHON_DEST_NAME = $${TARGET_NAME}.pyd
     }

#    contains(COMPILE_HOST_OS,Windows){
        #work around that qmake DESTDIR and other commands do not work with android make on windows (Qt 5.12.2)
        # copy built .so file to shared libs directory
        #bpythontarget.depends = $$shell_path($$OUT_PWD/$${PYTHON_SRC_NAME})
        #path just has to be something that can stat successfully
#        bpythontarget.path = $$shell_quote($$shell_path($$PWD/dummyfileforqmakebug.txt))
#        export(bpythontarget.path)
#        bpythontarget.commands = @echo copy to shared lib $${PYTHON_SRC_DIR}/$${PYTHON_SRC_NAME} && $$quote(xcopy $$quote( $$shell_path($$OUT_PWD/$${PYTHON_SRC_NAME}) $$shell_path($${PYTHON_SRC_DIR}/$${PYTHON_SRC_NAME})$$escape_expand(\n\t)))
#        export(bpythontarget.commands)

        # rename python lib and move to deploy python libs directory
        #path just has to be something that can stat successfully
#        bpythontarget2.path =  $$shell_quote($$shell_path($$PWD/dummyfileforqmakebug.txt))
#        bpythontarget2.target = mytarget2
#        export(bpythontarget2.path)
#        bpythontarget2.commands = @echo moving to $${DEST_PYTHON_DLL_DIR}/$${PYTHON_DEST_NAME} && $$quote( rm $$shell_path($${DEST_PYTHON_DLL_DIR}/$${PYTHON_DEST_NAME})) && $$quote(cp -f -R  $$quote( $$shell_path($${PYTHON_SRC_DIR}/$${PYTHON_SRC_NAME})  $$shell_path($${DEST_PYTHON_DLL_DIR}/$${PYTHON_DEST_NAME})$$escape_expand(\n\t)))
#        export(bpythontarget2.commands)

#        QMAKE_EXTRA_TARGETS += TARGET bpythontarget bpythontarget2
#        export(QMAKE_EXTRA_TARGETS)
#        INSTALLS += bpythontarget bpythontarget2
#    }

    contains(QMAKE_HOST.os,Windows){
        #this actually works on Windows.. TODO: find a way to commonize copy or move commands
        #because we cannot define DESTDIR.  Due to Qt bug we copy twice.. once to build directory and once to dest dir
        SRCDIR_WIN = $${OUT_PWD}/$${PYTHON_SRC_NAME}
        SRCDIR_WIN ~= s,/,\\,g
        DESTDIR_WIN1 = $${DEST_PYTHON_DLL_DIR}$${PYTHON_DEST_NAME}
        DESTDIR_WIN1 ~= s,/,\\,g

        PYTHON_LIB_COPY_CMD1 = copy /y $${SRCDIR_WIN} $${DESTDIR_WIN1}
        QMAKE_POST_LINK += $$quote(cmd /c $${PYTHON_LIB_COPY_CMD1}$$escape_expand(\n\t))
        message(**python dll first copy cmd  $$quote(cmd /c $${PYTHON_LIB_COPY_CMD1}$$escape_expand(\n\t)))

        DESTDIR_WIN2 = $${PYTHON_SRC_DIR}/$${PYTHON_SRC_NAME}
        DESTDIR_WIN2 ~= s,/,\\,g
        PYTHON_LIB_COPY_CMD2 = cmd /c copy /y $${SRCDIR_WIN} $${DESTDIR_WIN2}
        QMAKE_POST_LINK += $$quote(cmd /c $${PYTHON_LIB_COPY_CMD2}$$escape_expand(\n\t))
    }

    !contains(QMAKE_HOST.os,Windows){
        PYTHON_LIB_COPY_CMD = cp -f $${PYTHON_SRC_DIR}/$${PYTHON_SRC_NAME} $${DEST_PYTHON_DLL_DIR}$${PYTHON_DEST_NAME}
        QMAKE_POST_LINK += $$quote($${PYTHON_LIB_COPY_CMD})
        message("**python dll copy cmd $${PYTHON_LIB_COPY_CMD}")
    }
}

win32:{
    CONFIG(debug, debug|release){
        PYTHON_SRC_NAME = $${TARGET_NAME}_d.dll
        PYTHON_DEST_NAME = $${TARGET_NAME}_d.pyd
     }

     CONFIG(release, debug|release){
        PYTHON_SRC_NAME = $${TARGET_NAME}.dll
        PYTHON_DEST_NAME = $${TARGET_NAME}.pyd
     }

    #copydata.commands = copy $${PYTHON_SRC_DIR}/$${PYTHON_SRC_NAME} $${DEST_PYTHON_DLL_DIR}/$${PYTHON_DEST_NAME}
    PYTHON_LIB_COPY_CMD = cp $${PYTHON_SRC_DIR}/$${PYTHON_SRC_NAME} $${DEST_PYTHON_DLL_DIR}/$${PYTHON_DEST_NAME}
    QMAKE_POST_LINK += $$quote($${PYTHON_LIB_COPY_CMD})
}

