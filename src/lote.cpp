#include "../define/lote.h"
#include "../define/cursor.h"
#include "../define/proceso.h"
#include <chrono>
#include <cstdio>
#include <ostream>
#include <thread>

unsigned long Lote::tiempoTotal = 0;

Lote::Lote()
{ 
    procActual = nullptr;
}

// Constructor copy
Lote::Lote(const Lote& lote)
{

    this->ID = lote.ID;
    this->IDsUsados = lote.IDsUsados;
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

bool Lote::iniciarCaptura()
{
    char aux = 'n';
    procActual = nullptr;
    do {
        if (this->procPend.size() == BATCH_MAX_CAPACITY)
            return true;
        capturarProceso();
        std::cout << Cursor::Cursor::colorText(CYAN, "¿Desea capturar otro "
                                                     "proceso? s = si, "
                                                     "cualquier otra tecla "
                                                     "= no: ");
        std::cin >> aux;
        std::cin.ignore();
    } while (aux == 's' || aux == 'S');
    return false;
}

std::vector<Proceso>& Lote::getProcesosPendientes() const
{ return this->getProcesosPendientes(); }

void Lote::getProcesosTerminados()
{
    for (size_t i = 0; i < this->procTerm.size(); ++i) {
        std::cout << "Proceso #" << i + 1 << std::endl
                  << "ID: " << this->procTerm[i].getID() << std::endl
                  << "Nombre: " << this->procTerm[i].getNombre() << std::endl
                  << "Operación: " << this->procTerm[i].getOperacion()
                  << "Resultado: " << this->procTerm[i].getResultado()
                  << std::endl
                  << "Tiempo: " << this->procTerm[i].getTiempoMax()
                  << std::endl << std::endl;
    }
}

Proceso* Lote::getProcesoActual() const
{ return this->procActual; }

const unsigned long& Lote::getID() const
{ return this->ID; }

void Lote::setID(const unsigned long &ID)
{ this->ID = ID; }

void Lote::capturarProceso()
{
    Proceso aux;
    // Captura de ID
    capturarCampo("Ingrese el ID del proceso: ",
                  "ERROR: el ID no es válido, debe de ser un número positivo "
                  "del 1 al 99999", aux, &Proceso::setID, &this->IDsUsados);
    // Captura nombre
    capturarCampo("Ingrese el nombre del programador: ",
                  "ERROR: el nombre debe de contener al menos 3 caracteres "
                  "alfanuméricos (no debe de comenzar con un número)",
                  aux, &Proceso::setNombre);
    // Captura de operación
    capturarCampo("Ingrese la operación a realizar: ",
                  "ERROR: La operación ingresada no es válida. El formato "
                  "debe de ser \"int [+|-|/|%/*]\" int", aux,
                  &Proceso::setOperacion);
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
    std::cout << Cursor::Cursor::colorText(VERDE, msj);
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((proc.*metodo)(aux, IDs))
            break;
        if (!unaVez) {
            unaVez = true;
            Cursor::rmLine();
        }
        else
            Cursor::rmLine(2);
        std::cout << Cursor::Cursor::colorText(ROJO, msjError, true)
                  << std::endl;
        std::cout << Cursor::Cursor::colorText(VERDE, msj);
    }
}

void Lote::capturarCampo(std::string msj, std::string msjError,
                         Proceso& proc,
                         bool(Proceso::*metodo)(const std::string&))
{
    std::string aux;
    bool unaVez = false;
    std::cout << Cursor::colorText(VERDE, msj);
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((proc.*metodo)(aux))
            break;
        if (!unaVez) {
            unaVez = true;
            Cursor::rmLine();
        }
        else
            Cursor::rmLine(2);
        std::cout << Cursor::colorText(ROJO, msjError, true) << std::endl;
        std::cout << Cursor::colorText(VERDE, msj);
    }
}

void Lote::ejecutarProcesos()
{
    unsigned long cont;
    Frame pendientes(1, 12, FIELD_WIDTH * 2, 15, AMARILLO);
    Frame actual(FIELD_WIDTH * 2 + 2, 12, FIELD_WIDTH * 6, 15, VERDE);
    Frame terminados(FIELD_WIDTH * 8 + 3, 12, FIELD_WIDTH * 5, 15, CYAN);
   
    imprimirVentanas(&pendientes, &actual, &terminados);
    Cursor::gotoxy(72, 3);
    std::cout << "Tiempo total transcurrido (segundos): ";
    while (this->procPend.size()) {
        this->procActual = new Proceso(this->procPend.front());
        this->procPend.erase(this->procPend.begin());

        pendientes.rmContent();
        imprimirVentanas(&pendientes);
        for (size_t i = 0; i < this->procPend.size(); ++i)
            llenarMarco(pendientes, this->procPend[i], false, false);
               
        cont = this->procActual->getTiempoMax();
        while (cont--) {
            actual.rmContent();
            imprimirVentanas(nullptr, &actual);
            llenarMarco(actual, *this->procActual, true, false);
            Cursor::gotoxy(72, 4);
            std::cout << "          ";
            Cursor::gotoxy(72, 4);
            std::cout << Lote::tiempoTotal;
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++Lote::tiempoTotal;
            this->procActual->setTiempoRes(cont);
            this->procActual->setTiempoTrans(
                                       this->procActual->getTiempoTrans() + 1);
        }

        this->procActual->calculate();
        llenarMarco(terminados, *this->procActual, false, true);
        this->procTerm.push_back(*this->procActual);
        delete this->procActual; this->procActual = nullptr;
    }
    Cursor::gotoxy(72, 4);
    std::cout << Lote::tiempoTotal;
    pendientes.rmContent();
    actual.rmContent();
    terminados.rmContent();
    imprimirVentanas(&pendientes, &actual, &terminados);
}

void Lote::llenarMarco(Frame& marco, Proceso& proc, bool actual, bool term)
{
    marco.print(proc.getNombre(), BLANCO, false, FIELD_WIDTH);
    if (!actual && !term)
        marco.print(std::to_string(proc.getTiempoMax()), BLANCO, true,
                    FIELD_WIDTH);
    else if (actual || term) {
        marco.print(std::to_string(proc.getID()), BLANCO, false, FIELD_WIDTH);
        marco.print(proc.getOperacion(), BLANCO, false, FIELD_WIDTH);
        marco.print(std::to_string(proc.getTiempoMax()), BLANCO, false,
                    FIELD_WIDTH);
    }
    if (actual) {
        marco.print(std::to_string(proc.getTiempoRes()), BLANCO, false,
                    FIELD_WIDTH);
        marco.print(std::to_string(proc.getTiempoTrans()), BLANCO, true,
                    FIELD_WIDTH);
    }
    else if (term)
        marco.print(std::to_string(proc.getResultado()), BLANCO, true,
                    FIELD_WIDTH);
}

void Lote::imprimirVentanas(Frame* pend, Frame* act, Frame* term)
{
    if (pend) {
        pend->print("procesos      pendientes:", BLANCO, true);
        pend->print("NOM     TMPM    ", BLANCO, true);
    }
    if (act) {
        act->print("procesos actual:", BLANCO, true);
        act->print("NOM     ID      OP      TMPM    TMPR    TMPT    ",
                   BLANCO, true);
    }
    if (term){
        term->print("procesos terminados:", BLANCO, true);
        term->print("NOM     ID      OP      TMPM    RES     ", BLANCO, true);
    }
}
