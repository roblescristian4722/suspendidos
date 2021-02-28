#ifndef CURSOR_H
#define CURSOR_H
#if defined(_WIN64) || defined(_WIN32)
    #define CLRSCR "cls"
#else
    #define CLRSCR "clear"
#endif

#define AZUL '4'
#define ROJO '1'
#define MORADO '5'
#define VERDE '2'
#define AMARILLO '3'
#define CYAN '6'
#define BLACK '0'

#include <iostream>
#include <stdio.h>
#include <regex>

// Mueve el cursor de la terminal a una coordenada x,y
void gotoxy(int x,int y);
// Limpia la pantalla de la terminal
void clrscr();
// Muestra el cursor de la terminal
void showCursor();
// Oculta el cursor de la terminal
void hideCursor();
// Retorna una cadena pintada de un color
std::string colorText(unsigned char color, std::string msg,
                      bool foreground = true);

#endif // CURSOR_H
