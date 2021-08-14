

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
