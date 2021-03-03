#ifndef PROCESADOR_LOTES_H
#define PROCESADOR_LOTES_H
#include <iostream>
#include <map>
#include <vector>
#include "lote.h"
#include "cursor.h"

class ProcesadorLotes
{
private:
    std::vector<Lote> lotesPendientes;
    Lote* loteActual;
    std::vector<Lote>lotesTerminados;
    std::map<std::string, bool> IDsUsados;
    unsigned long tiempoTotal;

    // Ejecuta un búcle hasta que la captura del lote sea válida (hasta que se
    // retorne un true)
    void capturarLote();
    // Captura el ID de un lote, se ejecuta hasta que la captura del ID sea
    // correcta (hasta que el setID del lote retorne true)
    void capturarID(Lote& lote, bool(Lote::*metodo)
                    (const std::string&, std::map<std::string,bool>*),
                    std::map<std::string, bool>* IDs);
    // Se itera por el vector de lotes pendientes y se ejecutan en orden para
    // después guardarlos en el vector de lotes terminados
    void ejecutarLotes();

public:
    ProcesadorLotes();
    ~ProcesadorLotes();

    // Se comienza con la captura y posteriormente la ejecición de lotes
    void iniciar();
};

#endif // PROCESADOR_LOTES_H
