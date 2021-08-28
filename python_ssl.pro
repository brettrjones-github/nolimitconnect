# Global
TEMPLATE = lib
TARGET_NAME = _ssl

include(config_python_lib.pri)

include(config_opensslp_include.pri)

include(python_ssl.pri)

include(config_link.pri)

include(config_python_core_link.pri)

LIBS +=  $${STATIC_LIB_PREFIX}bz2$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}curl$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}depends$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}gnu$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}corelib$${STATIC_LIB_SUFFIX}
LIBS +=  $${STATIC_LIB_PREFIX}crossguid$${STATIC_LIB_SUFFIX}

include(config_ssl_link.pri)

include(config_move_python_lib.pri)





