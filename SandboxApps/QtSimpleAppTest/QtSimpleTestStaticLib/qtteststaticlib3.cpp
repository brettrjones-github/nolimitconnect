//#include <PktLib/VxCommon.h>
//#include <CoreLib/VxGUID.h>
//#include <CoreLib/config_corelib.h>
//#include <GoTvDependLibrariesConfig.h>
#include <GoTvCompilerConfig.h>

#include "qtteststaticlib3.h"
#include "qdebug.h"

QtTestStaticLib::QtTestStaticLib()
{
}

void QtTestStaticLib::printStuff()
{
    qWarning() << "printing static stuff ";
    // printf("printing static stuff ");
}


int QtTestStaticLib::getStaticValue()
{
    return 1;
}
