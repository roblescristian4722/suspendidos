#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <regex>
#include "cursor.h"
#if _WIN64 | _WIN32
    #include <Windows.h>
    #define SLEEP(x) Sleep(x);
#else
    #define SLEEP(x) std::this_thread::sleep_for(std::chrono::milliseconds(x));
#endif

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
    
    std::string aux;
    std::cout << "XD" << std::endl;
    std::regex validacion("(-?\\s*\\d\\s*[-|+|*|/|%]\\s*-?\\s*\\d)");
    while (1){
        system("clear");
        
        std::getline(std::cin, aux);
        std::cout << aux << ": " <<  std::regex_match(aux, validacion)
                  << std::endl;
        std::cin.get();
    }

    showCursor();
}
