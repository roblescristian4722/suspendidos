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
    
    Cursor::gotoxy(1, 31);
    std::cout << "Presione ENTER para continuar..." << std::endl;
    std::cin.get();
    p.mostrarResultados(1, 33);
}
