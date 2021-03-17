#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <regex>
#include "../define/cursor.h"
#include "../define/proceso.h"
#include "../define/lote.h"

bool test(int n)
{ return n; }

int main()
{ 
    Lote l;
    l.iniciar();

    Cursor::gotoxy(1, 31);
}
