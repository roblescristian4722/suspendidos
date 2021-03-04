#include "../define/procesadorLotes.h"

ProcesadorLotes::ProcesadorLotes()
{
    this->loteActual = nullptr;
    this->tiempoTotal = 0;
}

ProcesadorLotes::~ProcesadorLotes()
{
    if (this->loteActual)
        delete this->loteActual;
    this->loteActual = nullptr;
}

void ProcesadorLotes::iniciar()
{
    char aux = 'n';
    do {
        capturarLote();
        std::cout << Cursor::colorText(MORADO, "¿Desea capturar otro lote? s = si, "
                                     "cualquier otra tecla = no: ");
        std::cin >> aux;
        std::cin.ignore();
    } while (aux == 's' || aux == 'S');
    ejecutarLotes();
}

void ProcesadorLotes::capturarLote()
{
    Lote aux;

    // Se captura el ID del lote
    capturarID(aux, &Lote::setID, &this->IDsUsados);
    std::cout << "Iniciando captura de procesos del lote " 
              << aux.getID() << "..." << std::endl;
    aux.iniciarCaptura();
    // Se añade a la cola de lotes pendientes
    this->lotesPendientes.push_back(aux);
}

void ProcesadorLotes::ejecutarLotes()
{
    std::string aux;
    Cursor::clrscr();
    Frame lotes(1, 1, 10, 5, CYAN);
    while (this->lotesPendientes.size()) {
        this->loteActual = new Lote(this->lotesPendientes.front());
        this->lotesPendientes.erase(this->lotesPendientes.begin());
        
        for (size_t i = 0; i < this->lotesPendientes.size(); ++i) {
            lotes.printNum(this->lotesPendientes[i].getID(), BLANCO, true);
        }
        Cursor::gotoxy(1, 30);

        std::cout << "Iniciando ejecución de procesos del lote "
                  << this->loteActual->getID() << "..." << std::endl;
        this->loteActual->ejecutarProcesos();

        this->lotesTerminados.push_back(*this->loteActual);
        delete this->loteActual; this->loteActual = nullptr;
    }
}

void ProcesadorLotes::capturarID(Lote& lote, bool(Lote::*metodo)
                              (const std::string&, std::map<std::string,bool>*),
                               std::map<std::string, bool>* IDs)
{
    std::string aux;
    bool unaVez = false;
    std::cout << Cursor::colorText(VERDE, "Ingrese el ID del lote: ");
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((lote.*metodo)(aux, IDs)) {
            break;
        }
        if (!unaVez) {
            unaVez = true;
            Cursor::rmLine();
        }
        else
            Cursor::rmLine(2);
        std::cout << Cursor::colorText(ROJO, "ERROR: ID no válido, debe de ser un "
                               "número único entre 1 y 99999", true)
                  << std::endl;
        std::cout << Cursor::colorText(VERDE, "Ingrese el ID del lote: ");
    }
}
