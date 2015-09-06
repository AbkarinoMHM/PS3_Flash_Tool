#include <QByteArray>
#include "misc.h"


char* Misc::ByteSwap(char byteArray[], int length)
{
    int counter = 0;
    for (counter = 0; counter < length / 2; counter++)
    {
        int val1 = byteArray[counter * 2];
        int val2 = byteArray[(counter * 2) + 1];
        byteArray[counter * 2] = val2;
        byteArray[(counter * 2) + 1] = val1;
    }

    return byteArray;
}
