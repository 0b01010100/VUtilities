#include <vconsole.h>
#include <stdio.h>
#include <assert.h>
int main()
{
	printf("wuhdwo");
    VConsole_SetColor(VCONSOLE_COLOR_RED, VCONSOLE_COLOR_BROWN, VCONSOLE_ASCII_ITALIC);
    fprintf(stderr,"wuhdwo");

}