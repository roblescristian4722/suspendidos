#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <regex>
#include "../define/cursor.h"
#include "../define/proceso.h"
#include "../define/lote.h"
#include "../define/procesadorLotes.h"

bool test(int n)
{ return n; }

int main()
{ 
    ProcesadorLotes p;
    p.iniciar();

    Cursor::gotoxy(1, 25);
    std::cout << "presione una tecla para continuar..." << std::endl;
    std::cin.get();
}
