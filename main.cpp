#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <regex>
#include "cursor.h"
#include "proceso.h"
#include "lote.h"
#include "procesadorLotes.h"
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
    */

    Proceso proc;
    std::cout << proc.setNombre("2425234") << std::endl
              << proc.setID("asdf34") << std::endl
              << proc.setOperacion("3iglkh") << std::endl
              << proc.setTiempoMax("erkjg") << std::endl;

    Lote lote;
    lote.capturarLote();
    
}
