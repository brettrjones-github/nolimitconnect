
# a subset of GoTvAll.pro to avoid rebuilding rarely changed libs

TEMPLATE     = subdirs
CONFIG += no_docs_target

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered


#SUBDIRS += $$PWD/GoTvOpenSslLib.pro

SUBDIRS += $$PWD/NoLimit_App.pro





