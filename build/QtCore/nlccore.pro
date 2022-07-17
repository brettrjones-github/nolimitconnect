TEMPLATE = subdirs

# build core of gotv and all dependent libs

qtHaveModule(widgets) {
    SUBDIRS += \
        dependlibs \
        gotvcoreapp
}
