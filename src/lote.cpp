#include "../define/lote.h"
#include "../define/cursor.h"
#include "../define/proceso.h"
#include <chrono>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <ostream>
#include <thread>

unsigned long Lote::tiempoTotal = 0;
std::map<std::string, bool> Lote::idsUsed;

Lote::Lote()
{
    srand(time(NULL));
    this->procActual = nullptr;
}

Lote::Lote(const Lote& lote)
{
    this->id = lote.id;
    this->procPend = lote.procPend;
    this->procTerm = lote.procTerm;
    this->procActual = lote.procActual != nullptr
        ? new Proceso(lote.procActual->getId(),
                      lote.procActual->getNombre(),
                      lote.procActual->getOperacion(),
                      lote.procActual->getTiempoMax())
        : nullptr;
}

const Lote& Lote::operator=(const Lote &lote)
{
    this->id = lote.id;
    this->procPend = lote.procPend;
    this->procTerm = lote.procTerm;
    this->procActual = lote.procActual != nullptr
        ? new Proceso(lote.procActual->getId(),
                      lote.procActual->getNombre(),
                      lote.procActual->getOperacion(),
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

std::vector<Proceso>& Lote::getProcesosPendientes() const
{ return this->getProcesosPendientes(); }

Proceso* Lote::getProcesoActual() const
{ return this->procActual; }

const unsigned long& Lote::getId() const
{ return this->id; }

void Lote::printProcesosTerminados()
{
    for (size_t i = 0; i < this->procTerm.size(); ++i) {
        std::cout << "Proceso #" << i + 1 << std::endl
                  << "ID: " << this->procTerm[i].getId() << std::endl
                  << "Nombre: " << this->procTerm[i].getNombre() << std::endl
                  << "Operación: " << this->procTerm[i].getOperacion()
                  << "Resultado: " << this->procTerm[i].getResultado()
                  << std::endl
                  << "Tiempo: " << this->procTerm[i].getTiempoMax()
                  << std::endl << std::endl;
    }
}

void Lote::setId(const unsigned long &id)
{ this->id = id; }

void Lote::iniciar()
{
    unsigned long proc = 0;
    unsigned long cont = 0;
    
    std::cout << Cursor::colorText(VERDE, "Lotes a capturar: ");
    std::cin >> proc;
    while (proc--)
        capturarProceso(++cont);
    ejecutarProcesos();
}

///////////////////////////////////////////////
/*          CREACIÓN DE PROCESO              */
///////////////////////////////////////////////
void Lote::capturarProceso(const unsigned long& cont)
{
    Proceso aux;
    unsigned int lote = cont / BATCH_MAX_CAPACITY;
    
    // Captura de lote
    if (cont % BATCH_MAX_CAPACITY)
        aux.setLote(++lote);
    else
        aux.setLote(lote);
    // Captura de ID
    aux.setId(std::to_string(cont));
    // Captura de operación
    aux.setOperacion(generarOperacionProceso());
    // Captura de tiempo máximo
    aux.setTiempoMax(std::to_string(generarTiempoProceso()));
    this->procPend.push_back(aux);
}

///////////////////////////////////////////////
/*      GENERACIÓN DE DATOS ALEATORIOS       */
///////////////////////////////////////////////
std::string Lote::generarOperacionProceso()
{   
    const short numOp = 5;
    char op[numOp] = { '+', '-', '*', '/', '%' };
    std::string tmp = "";

    tmp = std::to_string((rand() % 99) + 1);
    tmp += op[rand() % numOp];
    tmp += std::to_string((rand() % 99) + 1);
    return tmp;
}

unsigned long Lote::generarTiempoProceso()
{
    const short maxTmp = 10;
    unsigned long tmp = 1;
    tmp = (rand() % maxTmp) + 6;
    return tmp;
}

///////////////////////////////////////////////
/*           CAPTURA DE CAMPOS               */
///////////////////////////////////////////////
void Lote::capturarCampo(std::string msj, std::string msjError,
                         Proceso& proc,
                         bool(Proceso::*metodo)(const std::string&,
                                                std::map<std::string,bool>*),
                         std::map<std::string, bool>* idsUsed)
{
    std::string aux;
    bool unaVez = false;
    std::cout << Cursor::Cursor::colorText(VERDE, msj);
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((proc.*metodo)(aux, idsUsed))
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

///////////////////////////////////////////////
/*           EJECUCIÓN DE PROCESOS           */
///////////////////////////////////////////////
void Lote::ejecutarProcesos()
{
    unsigned int lotesRes = this->procPend.size() / BATCH_MAX_CAPACITY;
    unsigned int lotesTerm = 0;
    unsigned long cont = 0;
    bool jump;
    
    Cursor::clrscr();
    Frame pend(1, 5, FIELD_WIDTH * 3 + 2, 15, AMARILLO);
    Frame actual(FIELD_WIDTH * 3 + 4, 5, FIELD_WIDTH * 5, 8, VERDE);
    Frame term(FIELD_WIDTH * 8 + 5, 5, FIELD_WIDTH * 5, 25, CYAN);
    
    if (!(this->procPend.size() % BATCH_MAX_CAPACITY))
        lotesRes--;
    imprimirVentanas(&pend, &actual, &term);
    while (this->procPend.size()) {
        jump = false;
        this->procActual = new Proceso(this->procPend.front());
        this->procPend.erase(this->procPend.begin());

        pend.rmContent();
        imprimirVentanas(&pend);
        for (size_t i = 0; i < this->procPend.size(); ++i)
            if (this->procPend[i].getLote() == (lotesTerm + 1))
                llenarMarco(pend, this->procPend[i], false, false);
        
        cont = this->procActual->getTiempoMax()
               - this->procActual->getTiempoTrans();
        jump = escTeclado(cont);
        while (cont--) {
            actual.rmContent();
            imprimirVentanas(nullptr, &actual);
            llenarMarco(actual, *this->procActual, true, false);

            Cursor::gotoxy(1, 2);
            Cursor::rmLine(2);
            std::cout << "Lotes pendientes: " << lotesRes << std::endl
                      << "Tiempo transcurrido: " << Lote::tiempoTotal;
          
            std::cout.flush();
            jump = escTeclado(cont);
            if (jump)
                break;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++Lote::tiempoTotal;
            this->procActual->setTiempoRes(cont);
            this->procActual->setTiempoTrans(
                                       this->procActual->getTiempoTrans() + 1);
        }

        if (!jump) {
            this->procActual->calculate();
            llenarMarco(term, *this->procActual, false, true);
            this->procTerm.push_back(*this->procActual);
            if (!(this->procTerm.size() % BATCH_MAX_CAPACITY)){
                lotesRes--;
                lotesTerm++;
            }
        }
        delete this->procActual; this->procActual = nullptr;
    }
    pend.rmContent(); actual.rmContent();
    imprimirVentanas(&pend, &actual);
}

bool Lote::escTeclado(unsigned long &cont)
{
    unsigned char input;
    if (kbhit()) {
        input = getch();
        switch (input) {
        case 'i': case 'I':
            return inter(cont);
        case 'p': case 'P':
            pausa();
            return false;
        case 'e': case 'E':
            error(cont);
            return false;
        default:
            return false;
        }
    }
    return false;
}

bool Lote::inter(unsigned long &cont)
{
    std::vector<Proceso>::iterator it = this->procPend.begin();
    unsigned int lote = it->getLote();
    if ((this->procTerm.size() % BATCH_MAX_CAPACITY) != BATCH_MAX_CAPACITY - 1){
        cont = 0;
        for (it; it->getLote() == lote && it != this->procPend.end(); it++);
        this->procPend.insert(it, *this->procActual);
        return true;
    }
    return false;
}

void Lote::error(unsigned long &cont)
{
    
    cont = 0;

}

void Lote::pausa()
{
    unsigned char input;
    Cursor::gotoxy(1, 3);
    std::cout << Cursor::colorText(MORADO,
                        "Ejecucion pausada - Presione \"c\" para continuar");
    while (1) {
        if (kbhit()) {
            input = getch();
            if (input == 'c' || input == 'C') {
                Cursor::rmLine();
                break;
            }
        }
    }
}

///////////////////////////////////////////////
/*           IMPRESIÓN DE VENTANAS           */
///////////////////////////////////////////////
void Lote::llenarMarco(Frame& marco, Proceso& proc, bool actual, bool term)
{
    marco.print(std::to_string(proc.getId()), BLANCO, false, FIELD_WIDTH);
    if (!actual && !term) {
        marco.print(std::to_string(proc.getTiempoMax()), BLANCO, false,
                    FIELD_WIDTH);
        marco.print(std::to_string(proc.getTiempoTrans()), BLANCO, true,
                    FIELD_WIDTH);
    }
    else if (actual || term) {
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
    else if (term) {
        marco.print(std::to_string(proc.getResultado()), BLANCO, false,
                    FIELD_WIDTH);
        marco.print(std::to_string(proc.getLote()), BLANCO, true,
                    FIELD_WIDTH);
    }
}

void Lote::imprimirVentanas(Frame* pend, Frame* act, Frame* term)
{
    if (pend) {
        pend->print("procesos pendientes:", BLANCO, true);
        pend->print("ID      TMPM    TT", BLANCO, true);
    }
    if (act) {
        act->print("procesos actual:", BLANCO, true);
        act->print("ID      OP      TMPM    TMPR    TMPT",
                   BLANCO, true);
    }
    if (term){
        term->print("procesos terminados:", BLANCO, true);
        term->print("ID      OP      TMPM    RES     LOTE", BLANCO, true);
    }
}
