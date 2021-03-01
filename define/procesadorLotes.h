#ifndef PROCESADOR_LOTES_H
#define PROCESADOR_LOTES_H
#include <iostream>
#include <map>
#include <vector>
#include "lote.h"

class ProcesadorLotes
{
private:
    std::vector<Lote> lotesPendientes;
    std::map<std::string, unsigned long> IDsUsados;
    std::vector<Lote>lotesTerminados;
    
public:
    void capturarLote();
    void capturarID(Lote& lote, bool(Lote::*metodo)
                    (const std::string&, std::map<std::string,bool>),
                    std::map<std::string, bool>* IDs);

    ProcesadorLotes();
    ~ProcesadorLotes();
};

#endif // PROCESADOR_LOTES_H
