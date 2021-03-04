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
#include <regex>
#include <stdio.h>

struct Cursor
{
    // Mueve el cursor de la terminal a una coordenada x,y
    static void gotoxy(int x,int y);
    // Limpia la pantalla de la terminal
    static void clrscr();
    // Muestra el cursor de la terminal
    static void showCursor();
    // Oculta el cursor de la terminal
    static void hideCursor();
    // Retorna una cadena pintada de un color
    static std::string colorText(unsigned char color, std::string msg,
                          bool foreground = true);
    
    // Elimina n cantidad de líneas en pantalla comenzando por la posición actual
    // del puntero
    static void rmLine(unsigned int n = 1);
    // Dibuja una línea horizontal en la terminal
    static void drawXLine(int x, int y, int w, char color = BLANCO,
                   bool foreground = false, std::string  caracter = "*");
    // Dibuja una línea vertical en la terminal
    static void drawYLine(int x, int y, int h, char color = BLANCO,
                   bool foreground = false, std::string  caracter = "*");

};

class Frame: public Cursor
{
private:
    unsigned int heightFrame;
    unsigned int widthFrame;
    unsigned int xFrame;
    unsigned int yFrame;
    char contentColor;
    char color;
    bool foreground;
    std::string simbol;
    unsigned int xPos;
    unsigned int yPos;

public:
    Frame(int x = 0, int y = 0, int w = 0, int h = 0, char color = BLANCO,
           bool foreground = true, std::string simbol = "*");
    ~Frame();

    // Dibuja un marco en la terminal y guarda la información brindada para
    // imprimir el contenido dentro del marco
    void drawFrame(int x, int y, int w, int h, char color = BLANCO,
                   bool foreground = false, std::string caracter = "*");
    // Imprime un mensaje dentro del marco
    void print(std::string msj, char color = BLANCO,
               bool newl = false);
};

#endif // CURSOR_H
