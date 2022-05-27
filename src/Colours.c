#include "Colours.h"

void SetCol(const char *colStr)
{
    printf("\033[1;31m");
    //printf("%s", colStr);
}

void ResetCol()
{
    printf("\033[0m");
}