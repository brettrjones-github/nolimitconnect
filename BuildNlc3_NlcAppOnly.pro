
# a subset of No Limit Connect o to avoid rebuilding rarely changed libs

TEMPLATE     = subdirs
CONFIG += no_docs_target

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered


#SUBDIRS += $$PWD/NlcOpenSslLib.pro

SUBDIRS += $$PWD/NoLimit_App.pro





