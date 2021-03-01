#ifndef LOTE_H
#define LOTE_H
#include <iostream>
#include <vector>
#include "proceso.h"
#include "cursor.h"

class Lote
{
private:
    unsigned long ID;
    std::vector<Proceso> procPend;
    Proceso* procActual;
    std::vector<Proceso> procTerm;

    void capturarCampo(std::string msj, std::string msjError,
                       Proceso& proc,
                       bool(Proceso::*metodo)(const std::string&));
public:
    Lote();
    ~Lote();

    void capturarLote();
};

#endif // LOTE_H
