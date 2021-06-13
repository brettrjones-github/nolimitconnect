requires(qtHaveModule(gui))

TEMPLATE     = subdirs

SUBDIRS += QtSimpleTestApp
SUBDIRS += QtSimpleTestStaticLib
SUBDIRS += QtSimpleTestSharedLib

# what subproject depends on others
QtSimpleTestApp.depends = QtSimpleTestStaticLib QtSimpleTestSharedLib
