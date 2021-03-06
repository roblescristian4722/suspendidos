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

    // Se itera por el vector de lotes pendientes y se ejecutan en orden para
    // después guardarlos en el vector de lotes terminados
    void ejecutarLotes();

public:
    ProcesadorLotes();
    ~ProcesadorLotes();

    // Muestra una lista con todos los procesos y lotes al igual que sus
    // resultados y datos generales
    void mostrarResultados(unsigned int x = 1, unsigned int y = 31);
    // Se comienza con la captura y posteriormente la ejecición de lotes
    void iniciar();
};

#endif // PROCESADOR_LOTES_H
