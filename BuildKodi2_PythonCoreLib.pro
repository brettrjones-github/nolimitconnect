

TEMPLATE     = subdirs
CONFIG += no_docs_target

CONFIG(debug, debug|release){
    message(ERROR Only Build Python In Release Mode.. Debug Not Supported)
}

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

include(config_os_detect.pri)
#NOTE: be sure you have build Build1_PythonDepends first
SUBDIRS += $$PWD/python_pythoncore.pro
