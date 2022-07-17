#pragma once
#include <AppDependLibrariesConfig.h>
//#define OC_FACTOR_INTO_H_AND_CC 1
#define OC_NEW_STYLE_INCLUDES 1
#define OC_INLINE NLC_INLINE

#ifdef _MSC_VER
# pragma warning( disable : 4812 ) // obsolete declaration style
# pragma warning( disable : 4522 ) // multiple assignment operators specified
# pragma warning( disable : 4099 ) // type name first seen using 'class' now seen using 'struct'
# pragma warning( disable : 4141 ) // 'inline': used more than once
# pragma warning( disable : 4396 ) // inline specifier cannot be used when a friend declaration refers to a specialization of a function template
#endif // _MSC_VER
