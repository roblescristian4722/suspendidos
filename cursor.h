#ifndef CURSOR_H
#define CURSOR_H
#if defined(_WIN64) || defined(_WIN32)
    #define CLRSCR "cls"
#else
    #define CLRSCR "clear"
#endif

#include <iostream>
#include <stdio.h>

// Mueve el cursor de la terminal a una coordenada x,y
void gotoxy(int x,int y);
// Limpia la pantalla de la terminal
void clrscr();
// Muestra el cursor de la terminal
void showCursor();
// Oculta el cursor de la terminal
void hideCursor();

#endif // CURSOR_H
