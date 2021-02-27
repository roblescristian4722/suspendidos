#include "cursor.h"

void gotoxy(int x,int y)
{ printf("%c[%d;%df",0x1B,y,x); }
void clrscr()
{ system(CLRSCR); }

void hideCursor()
{ printf("\e[?25l"); }

void showCursor()
{ printf("\e[?25h"); }

