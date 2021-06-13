#pragma once

#if defined(TARGET_OS_WINDOWS)
// Q_DECL_EXPORT is just the same (under Windows) as __declspec(dllexport) pragma
# include <QObject>
# if defined(MAKE_SHARED_LIB) && defined(TARGET_OS_WINDOWS)
#  define IMPORT_EXPORT Q_DECL_EXPORT
# else
#  define IMPORT_EXPORT
# endif
#endif

#if !defined(TARGET_OS_WINDOWS)
# define IMPORT_EXPORT
#endif

// absolutely must have Q_DECL_EXPORT defined when building or .lib is not generated because of empty export table
class IMPORT_EXPORT QtTestSharedLib
{

public:
    QtTestSharedLib();

    void printStuff();
    int getSharedValue();
};
