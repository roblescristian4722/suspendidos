#include "procesadorLotes.h"

ProcesadorLotes::ProcesadorLotes()
{}

ProcesadorLotes::~ProcesadorLotes()
{}

void ProcesadorLotes::capturarLote()
{

}

void ProcesadorLotes::capturarID(Lote& lote, bool(Lote::*metodo)
                              (const std::string&, std::map<std::string,bool>),
                               std::map<std::string, bool>* IDs)
{
    std::string aux;
    bool unaVez = false;
    std::cout << colorText(VERDE, "Ingrese el ID del lote: ");
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((lote.*metodo)(aux, *IDs)) {
            break;
        }
        if (!unaVez) {
            unaVez = true;
            rmLine();
        }
        else
            rmLine(2);
        std::cout << colorText(ROJO, "ERROR: ID no válido, debe de ser un "
                               "número único entre 1 y 99999", true)
                  << std::endl;
        std::cout << colorText(VERDE, "Ingrese el ID del lote: ");
    }
}
