#ifndef LOTE_H
#define LOTE_H
#include <iostream>
#include <queue>
#include "proceso.h"

class Lote
{
private:
    unsigned long ID;
    std::queue<Proceso> procPend;
    Proceso* procActual;
    std::queue<Proceso> procTerm;
public:
    Lote();
    ~Lote();

    void capturarLote();
};

#endif // LOTE_H
