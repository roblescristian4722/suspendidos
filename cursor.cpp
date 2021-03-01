#include "cursor.h"

void gotoxy(int x,int y)
{ printf("%c[%d;%df",0x1B,y,x); }
void clrscr()
{ system(CLRSCR); }

void hideCursor()
{ printf("\e[?25l"); }

void showCursor()
{ printf("\e[?25h"); }

std::string colorText(unsigned char color, std::string msg, bool foreground)
{
    std::string aux;
    // foreground ANSI: "\033[0(fondo negro);3{color}m{msg}\033[0m"
    if (foreground) {
        aux = "\033[0;3";
        aux += color;
    }
    // background ANSI: "\033[4{color fondo}m\033[1;30(color foreground negro)m
    //              {msg}\033[0m"
    else {
        aux = "\033[4";
        aux += color;
        aux += "m\033[1;30";
    }
    aux += 'm';
    aux += msg;
    aux += "\033[0m";
    return aux;
}

void rmLine(unsigned int n)
{
    while (n--) {
        // Borra la línea actual
        std::cout << "\033[2K";
        // Regresa a la línea anterior (sube 1 línea)
        std::cout << "\033[1A";
    }
}
