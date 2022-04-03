
# a subset of No Limit Connect  to avoid rebuilding rarely changed libs

TEMPLATE     = subdirs
CONFIG += no_docs_target

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered


#SUBDIRS += $$PWD/GoTvOpenSslLib.pro

SUBDIRS += $$PWD/NoLimit_AppWithKodi.pro





