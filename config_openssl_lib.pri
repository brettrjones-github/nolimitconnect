
# include(config_version.pri) # not needed unless building kodi
include(config_os_detect.pri)
include(config_compiler.pri)

include(config_sharedlib_openssl.pri)

include(config_link.pri)

INCLUDEPATH += $$PWD/DependLibs/libgnu

