#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include "cursor.h"


int main() { 
    gotoxy(20,50);
    std::cout<<"hola"<<std::endl;
    gotoxy(10,15);
    std::cout<<"mundo"<<std::endl;
    std::cin.get();
    hideCursor();
    std::cin.get();

    using namespace std::chrono_literals;
    std::cout << "Hello waiter\n" << std::flush;
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "Waited " << elapsed.count() << " ms\n";

    showCursor();
}
