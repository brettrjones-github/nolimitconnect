

TEMPLATE     = subdirs
CONFIG += no_docs_target

CONFIG(debug, debug|release){
    message(ERROR Only Build Python In Release Mode.. Debug Not Supported)
}

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

include(config_os_detect.pri)
#NOTE: be sure you have build Build1_PythonDepends and Build2_PythonCoreLib first

SUBDIRS += $$PWD/python_bz2.pro

SUBDIRS += $$PWD/python_ctypes.pro
SUBDIRS += $$PWD/python_ctypes_test.pro
SUBDIRS += $$PWD/python_elementtree.pro
SUBDIRS += $$PWD/python_hashlib.pro
SUBDIRS += $$PWD/python_sqlite3.pro
SUBDIRS += $$PWD/python_socket.pro
SUBDIRS += $$PWD/python_testcapi.pro

SUBDIRS += $$PWD/python_pyexpat.pro

SUBDIRS += $$PWD/python_select.pro
SUBDIRS += $$PWD/python_unicodedata.pro

SUBDIRS += $$PWD/python_ssl.pro

#SUBDIRS += $$PWD/python_xcopy_dlls.pro


#copydlls.commands = cmd c copy $OUT_PWD\*.so F:\nolimitconnect\build-pythonlibs\Android\armeabi-v7a
#copydepend.depends = copydlls
#QMAKE_EXTRA_TARGETS += copydepend copydlls


#SUBDIRS += $$PWD/python_bsddb.pro #FIXME does not build

#SUBDIRS += $$PWD/python_tk.pro #FIXME does not build
#SUBDIRS += $$PWD/python_tcl.pro #FIXME does not build
#SUBDIRS += $$PWD/python_tix.pro #FIXME does not build
#SUBDIRS += $$PWD/python_tkinter.pro #FIXME does not build


# sigh.. cannot get qt to exclude these when not builing windows so you will have to uncomment them
#win32 {
#    SUBDIRS += $$PWD/python_multiprocessing.pro #FIXME does not build
#    SUBDIRS += $$PWD/python_msi.pro
#    SUBDIRS += $$PWD/python_w9xpopen.pro
#    SUBDIRS += $$PWD/python_winsound.pro
#}

