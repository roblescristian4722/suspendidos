#include "../define/procesadorLotes.h"

ProcesadorLotes::ProcesadorLotes()
{
    this->loteActual = nullptr;
}

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
        this->lotesTerminados[i].getProcesosTerminados();
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
    Cursor::clrscr();
    Frame lotes(1, 1, 26, 10, AMARILLO);
    Frame actual(28, 1, 20, 10, VERDE);
    Frame terminados(50, 1, 20, 10, CYAN);
 
    lotes.print("lotes pendientes: (");
    lotes.print(std::to_string(this->lotesPendientes.size()));
    lotes.print("):", BLANCO, true);
    actual.print("lotes actual:", BLANCO, true);
    terminados.print("lotes terminados:", BLANCO, true);
    while (this->lotesPendientes.size()) {
        this->loteActual = new Lote(this->lotesPendientes.front());
        this->lotesPendientes.erase(this->lotesPendientes.begin());
        
        lotes.rmContent();
        lotes.print("lotes pendientes: (");
        lotes.print(std::to_string(this->lotesPendientes.size()));
        lotes.print("):", BLANCO, true);
        for (size_t i = 0; i < this->lotesPendientes.size(); ++i)
            lotes.printNum(this->lotesPendientes[i].getID(), BLANCO, true);

        actual.printNum(this->loteActual->getID());
        this->loteActual->ejecutarProcesos();

        terminados.printNum(this->loteActual->getID(), BLANCO, true);
        this->lotesTerminados.push_back(*this->loteActual);
        delete this->loteActual; this->loteActual = nullptr;
    }
    actual.rmContent();
    actual.print("lotes actual:", BLANCO, true);
}
