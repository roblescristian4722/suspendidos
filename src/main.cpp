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

    std::string aux;
    Proceso *p;
    while(1) {
        std::cout << "nuevo..." << std::endl;
        getline(std::cin, aux);
        p = new Proceso(aux);
        p->calculate();
        std::cout << " | res: " << p->getResultado() << std::endl;
        delete p;
    }
}
