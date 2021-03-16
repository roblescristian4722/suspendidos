#include "../define/procesadorLotes.h"

ProcesadorLotes::ProcesadorLotes()
{ this->loteActual = nullptr; }

ProcesadorLotes::~ProcesadorLotes()
{
    if (this->loteActual)
        delete this->loteActual;
    this->loteActual = nullptr;
}

void ProcesadorLotes::mostrarResultados(unsigned int x, unsigned int y)
{
    Cursor::gotoxy(x, y);
    std::cout << "RESULTADOS:" << std::endl;
    for (size_t i = 0; i < this->lotesTerminados.size(); ++i) {
        std::cout << "-------------------- LOTE "
                  << this->lotesTerminados[i].getID()
                  << " --------------------" << std::endl;
        this->lotesTerminados[i].printProcesosTerminados();
    }
}

void ProcesadorLotes::iniciar()
{
    Lote *aux;
    unsigned int cont = 1;
    bool overflow = false;

    do{
        aux = new Lote;
        aux->setID(cont);
        overflow = aux->iniciarCaptura();
        ++cont;
        this->lotesPendientes.push_back(*aux);
        delete aux;
        aux = nullptr;
    }while(overflow);
    ejecutarLotes();
}

void ProcesadorLotes::ejecutarLotes()
{
    std::string aux;

    Cursor::gotoxy(1, 2);
    Cursor::rmLine();
    std::cout << "Lotes pendientes: " << this->lotesPendientes.size();
    while (this->lotesPendientes.size()) {
        this->loteActual = new Lote(this->lotesPendientes.front());
        this->lotesPendientes.erase(this->lotesPendientes.begin());
        
        this->loteActual->ejecutarProcesos();

        this->lotesTerminados.push_back(*this->loteActual);
        delete this->loteActual; this->loteActual = nullptr;
    }
}
