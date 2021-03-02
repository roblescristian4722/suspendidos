#include "../define/lote.h"
#include "../define/cursor.h"
#include "../define/proceso.h"
#include <chrono>
#include <cstdio>
#include <ostream>
#include <thread>

Lote::Lote()
{ 
    procActual = nullptr;
    tiempoTotal = 0;
}

// Constructor copy
Lote::Lote(const Lote& lote)
{

    this->ID = lote.ID;
    this->IDsUsados = lote.IDsUsados;
    this->tiempoTotal = lote.tiempoTotal;
    this->procPend = lote.procPend;
    this->procTerm = lote.procTerm;
    this->procActual = lote.procActual != nullptr
        ? new Proceso(lote.procActual->getNombre(),
                      lote.procActual->getOperacion(),
                      lote.procActual->getID(),
                      lote.procActual->getTiempoMax())
        : nullptr;
}

const Lote& Lote::operator=(const Lote &lote)
{
    this->ID = lote.ID;
    this->IDsUsados = lote.IDsUsados;
    this->tiempoTotal = lote.tiempoTotal;
    this->procPend = lote.procPend;
    this->procTerm = lote.procTerm;
    this->procActual = lote.procActual != nullptr
        ? new Proceso(lote.procActual->getNombre(),
                      lote.procActual->getOperacion(),
                      lote.procActual->getID(),
                      lote.procActual->getTiempoMax())
        : nullptr;

    return *this;
}

Lote::~Lote()
{ 
    if (this->procActual)
        delete this->procActual;
    this->procActual = nullptr;
}

void Lote::iniciarCaptura()
{
    char aux = 'n';
    procActual = nullptr;
    do {
        capturarLote();
        std::cout << colorText(CYAN, "¿Desea capturar otro proceso? s = si, "
                                     "cualquier otra tecla = no: ");
        std::cin >> aux;
        std::cin.ignore();
    } while (aux == 's' || aux == 'S');
}

void Lote::getProcesosPendientes()
{
    for (size_t i = 0; i < this->procPend.size(); ++i) {
        std::cout << "Proceso #" << i + 1 << std::endl
                  << "ID: " << this->procPend[i].getID() << std::endl
                  << "Nombre: " << this->procPend[i].getNombre() << std::endl
                  << "Operación: " << this->procPend[i].getOperacion()
                  << std::endl
                  << "Tiempo: " << this->procPend[i].getTiempoMax()
                  << std::endl;
    }
}

void Lote::getProcesosTerminados()
{
    for (size_t i = 0; i < this->procTerm.size(); ++i) {
        std::cout << "Proceso #" << i + 1 << std::endl
                  << "ID: " << this->procTerm[i].getID() << std::endl
                  << "Nombre: " << this->procTerm[i].getNombre() << std::endl
                  << "Operación: " << this->procTerm[i].getOperacion()
                  << std::endl
                  << "Tiempo: " << this->procTerm[i].getTiempoMax()
                  << std::endl;
    }
}

Proceso* Lote::getProcesoActual() const
{ return this->procActual; }

const unsigned long& Lote::getID() const
{ return this->ID; }

bool Lote::setID(const std::string&ID, std::map<std::string, bool>* IDs)
{
    std::regex validacion("[1-9][0-9]{0,4}");
    if (std::regex_match(ID, validacion))
        if (IDs->find(ID) == IDs->end()) {
            (*IDs)[ID] = true;
            this->ID = std::stoul(ID);
            return true;
        }
    return false;
}

void Lote::capturarLote()
{
    Proceso aux;
    // Captura nombre
    capturarCampo("Ingrese el nombre del programador: ",
                  "ERROR: el nombre debe de contener al menos 3 caracteres "
                  "alfanuméricos (no debe de comenzar con un número)",
                  aux, &Proceso::setNombre);
    // Captura de operación
    capturarCampo("Ingrese la operación a realizar: ",
                  "ERROR: La operación ingresada no es válida. El formato "
                  "debe de ser \"int [+|-|/|%]\" int", aux,
                  &Proceso::setOperacion);
    // Captura de ID
    capturarCampo("Ingrese el ID del proceso: ",
                  "ERROR: el ID no es válido, debe de ser un número positivo "
                  "del 1 al 99999", aux, &Proceso::setID, &this->IDsUsados);
    // Captura de tiempo máximo
    capturarCampo("Ingrese el tiempo máximo estimado de ejecución: ",
                  "ERROR: el tiempo máximo de ejecución debe de ser un número "
                  "mayor a cero", aux, &Proceso::setTiempoMax);
    this->procPend.push_back(aux);
}

void Lote::capturarCampo(std::string msj, std::string msjError,
                         Proceso& proc,
                         bool(Proceso::*metodo)(const std::string&,
                                                std::map<std::string,bool>*),
                         std::map<std::string, bool>* IDs)
{
    std::string aux;
    bool unaVez = false;
    std::cout << colorText(VERDE, msj);
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((proc.*metodo)(aux, IDs)) {
            break;
        }
        if (!unaVez) {
            unaVez = true;
            rmLine();
        }
        else
            rmLine(2);
        std::cout << colorText(ROJO, msjError, true) << std::endl;
        std::cout << colorText(VERDE, msj);
    }
}

void Lote::capturarCampo(std::string msj, std::string msjError,
                         Proceso& proc,
                         bool(Proceso::*metodo)(const std::string&))
{
    std::string aux;
    bool unaVez = false;
    std::cout << colorText(VERDE, msj);
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((proc.*metodo)(aux)) {
            break;
        }
        if (!unaVez) {
            unaVez = true;
            rmLine();
        }
        else
            rmLine(2);
        std::cout << colorText(ROJO, msjError, true) << std::endl;
        std::cout << colorText(VERDE, msj);
    }
}

void Lote::ejecutarProcesos()
{
    unsigned long cont;
    std::vector<Proceso>::iterator it; 

    std::cout << "pendientes:" << std::endl;
    getProcesosPendientes();
    std::cout << "terminados:" << std::endl;
    getProcesosTerminados();

    while (this->procPend.size()) {
        this->procActual = new Proceso(this->procPend.front());
        this->procPend.erase(this->procPend.begin());

        cont = this->procActual->getTiempoMax();
        this->tiempoTotal += 0;

        while (cont--) {
            std::cout << "procesando " << this->procActual->getID()
                      << "..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));        
        }
        this->procTerm.push_back(*this->procActual);
        delete this->procActual; this->procActual = nullptr;
    }
    
    std::cout << std::endl << "pendientes:" << std::endl;
    getProcesosPendientes();
    std::cout << "terminados:" << std::endl;
    getProcesosTerminados();
}
