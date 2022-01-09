
# include(config_version.pri) # not needed unless building kodi
include(config_os_detect.pri)
include(config_compiler.pri)

include(config_sharedlib_python.pri)

include(config_link.pri)


INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14
INCLUDEPATH += $$PWD/DependLibs/Python-2.7.14/Include
INCLUDEPATH += $$PWD/DependLibs/libgnu

