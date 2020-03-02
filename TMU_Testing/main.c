#include <stdio.h>
#include <stdlib.h>
#include "Tester.h"

int main()
{
    Test_TMU_Init();
    Test_TMU_DeInit();
    Test_TMU_Start();
    Test_TMU_Stop();
    return 0;
}
