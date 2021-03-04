#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <regex>
#include "../define/cursor.h"
#include "../define/proceso.h"
#include "../define/lote.h"
#include "../define/procesadorLotes.h"
#if _WIN64 | _WIN32
    #include <Windows.h>
    #define SLEEP(x) Sleep(x);
#else
    #define SLEEP(x) std::this_thread::sleep_for(std::chrono::milliseconds(x));
#endif

bool test(int n)
{ return n; }

int main() { 
    /*gotoxy(20,50);
    std::cout<<"hola"<<std::endl;
    gotoxy(10,15);
    std::cout<<"mundo"<<std::endl;
    std::cin.get();
    hideCursor();
    std::cin.get();

    using namespace std::chrono_literals;
    std::cout << "Hello waiter\n" << std::flush;
    auto start = std::chrono::high_resolution_clock::now();
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    SLEEP(2000)
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "Waited " << elapsed.count() << " ms\n";
    ProcesadorLotes procLotes;
    procLotes.iniciar();
    */
    
    //ProcesadorLotes p;
    //p.iniciar();
    Frame f(1, 1, 5, 3, VERDE);
    f.clrscr();
    //drawFrame(1, 3, 10, 5, AZUL);
    f.print("Hola qué tal, cómo te va, me da alegría mirarte, la felicidad de hablarte", CYAN);
    //drawFrame(21, 10, 15, 10, CYAN);
    std::cin.get();
}
