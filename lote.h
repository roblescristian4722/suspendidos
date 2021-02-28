#ifndef LOTE_H
#define LOTE_H
#include <iostream>
#include <queue>
#include "proceso.h"

class Lote
{
private:
    std::queue<Proceso> procPend;
    Proceso* procActual;
    std::queue<Proceso> procTerm;
public:
    Lote();
    ~Lote();

    void capturarLote();
};

#endif // LOTE_H
