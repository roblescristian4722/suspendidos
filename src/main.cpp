#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <regex>
#include "../define/cursor.h"
#include "../define/process.h"
#include "../define/newprocesses.h"

bool test(int n)
{ return n; }

int main()
{ 
    NewProcesses l;
    l.init();

    Cursor::gotoxy(1, 31);
    std::cout << Cursor::colorText(
                 VERDE, "Fin de la ejecucion, presione ENTER para salir")
              << std::endl;
    std::cin.get();std::cin.get();
}
