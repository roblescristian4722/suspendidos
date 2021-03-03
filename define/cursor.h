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
#define NEGRO '0'
#define BLANCO '7'

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
// Retorna una cadena pintada de un color
std::string colorText(unsigned char color, char caracter,
                      bool foreground = true);

// Elimina n cantidad de líneas en pantalla comenzando por la posición actual
// del puntero
void rmLine(unsigned int n = 1);
// Dibuja una línea horizontal en la terminal
void drawXLine(int x, int y, int len, char color = BLANCO,
               std::string  caracter = "*");
// Dibuja una línea vertical en la terminal
void drawYLine(int x, int y, int len, char color = BLANCO,
               std::string  caracter = "*");
// Dibuja un marco en la terminal
void drawFrame(int x, int y, int w, int h, char color = BLANCO,
               std::string caracter = "*");


#endif // CURSOR_H
