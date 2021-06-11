#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <regex>
#include "../define/cursor.h"
#include "../define/process.h"
#include "../define/processManager.h"

bool test(int n)
{ return n; }

int main()
{
    ProcessManager m;
    m.init();

    Cursor::gotoxy(1, 31);
    std::cout << Cursor::colorText(
                 VERDE, "Fin de la ejecucion, presione ENTER para mostrar tiempos")
              << std::endl;
    std::cin.get();
    m.printFinished();
    std::cin.get();
    return 0;
}
