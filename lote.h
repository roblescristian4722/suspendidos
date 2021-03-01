#ifndef LOTE_H
#define LOTE_H
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
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
    Lote(const Lote& lote);
    ~Lote();
    
    void getProcesosPendientes();
    void getProcesosTerminados();
    Proceso* getProcesoActual() const;
    const unsigned long& getID() const;
    void capturarLote();

    void ejecutarProcesos();

    const Lote& operator =(const Lote& lote);
};

#endif // LOTE_H
