#include "../define/newprocesses.h"

unsigned long NewProcesses::lapsedTime = 0;
std::map<std::string, bool> NewProcesses::idsUsed;

NewProcesses::NewProcesses()
{
    srand(time(NULL));
    this->current = nullptr;
}

NewProcesses::NewProcesses(const NewProcesses& NewProcesses)
{
    this->id = NewProcesses.id;
    this->pending = NewProcesses.pending;
    this->finished = NewProcesses.finished;
    this->current = NewProcesses.current != nullptr
        ? new Process(NewProcesses.current->getId(),
                      NewProcesses.current->getName(),
                      NewProcesses.current->getOp(),
                      NewProcesses.current->getMaxTime())
        : nullptr;
}

const NewProcesses& NewProcesses::operator=(const NewProcesses &NewProcesses)
{
    this->id = NewProcesses.id;
    this->pending = NewProcesses.pending;
    this->finished = NewProcesses.finished;
    this->current = NewProcesses.current != nullptr
        ? new Process(NewProcesses.current->getId(),
                      NewProcesses.current->getName(),
                      NewProcesses.current->getOp(),
                      NewProcesses.current->getMaxTime())
        : nullptr;

    return *this;
}

NewProcesses::~NewProcesses()
{ 
    if (this->current)
        delete this->current;
    this->current = nullptr;
}

const std::vector<Process>& NewProcesses::getPending() const
{ return this->pending; }

Process* NewProcesses::getCurrent() const
{ return this->current; }

const unsigned long& NewProcesses::getId() const
{ return this->id; }

void NewProcesses::printFinished()
{
    for (size_t i = 0; i < this->finished.size(); ++i) {
        std::cout << "Process #" << i + 1 << std::endl
                  << "ID: " << this->finished[i].getId() << std::endl
                  << "Nombre: " << this->finished[i].getName() << std::endl
                  << "Operación: " << this->finished[i].getOp()
                  << "Resultado: " << this->finished[i].getResult()
                  << std::endl
                  << "Tiempo: " << this->finished[i].getMaxTime()
                  << std::endl << std::endl;
    }
}

void NewProcesses::setId(const unsigned long &id)
{ this->id = id; }

void NewProcesses::init()
{
    unsigned long proc = 0;
    unsigned long cont = 0;
    
    std::cout << Cursor::colorText(VERDE, "Processs a capturar: ");
    std::cin >> proc;
    while (proc--)
        obtainProcess(++cont);
    executeProcess();
}

///////////////////////////////////////////////
/*          CREACIÓN DE Process              */
///////////////////////////////////////////////
void NewProcesses::obtainProcess(const unsigned long& cont)
{
    Process aux;
    unsigned int NewProcesses = cont / MAX_JOB_AMOUNT;
    
    // Captura de ID
    aux.setId(std::to_string(cont));
    // Captura de operación
    aux.setOp(generateOp());
    // Captura de tiempo máximo
    aux.setMaxTime(std::to_string(generateTime()));
    this->pending.push_back(aux);
}

///////////////////////////////////////////////
/*      GENERACIÓN DE DATOS ALEATORIOS       */
///////////////////////////////////////////////
std::string NewProcesses::generateOp()
{   
    const short numOp = 5;
    char op[numOp] = { '+', '-', '*', '/', '%' };
    std::string tmp = "";

    tmp = std::to_string((rand() % 99) + 1);
    tmp += op[rand() % numOp];
    tmp += std::to_string((rand() % 99) + 1);
    return tmp;
}

unsigned long NewProcesses::generateTime()
{
    const short maxTmp = 10;
    unsigned long tmp = 1;
    tmp = (rand() % maxTmp) + 6;
    return tmp;
}

///////////////////////////////////////////////
/*           CAPTURA DE CAMPOS               */
///////////////////////////////////////////////
void NewProcesses::obtainField(std::string msj, std::string msjError,
                         Process& proc,
                         bool(Process::*metodo)(const std::string&,
                                                std::map<std::string,bool>*),
                         std::map<std::string, bool>* idsUsed)
{
    std::string aux;
    bool once = false;
    std::cout << Cursor::Cursor::colorText(VERDE, msj);
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((proc.*metodo)(aux, idsUsed))
            break;
        if (!once) {
            once = true;
            Cursor::rmLine();
        }
        else
            Cursor::rmLine(2);
        std::cout << Cursor::Cursor::colorText(ROJO, msjError, true)
                  << std::endl;
        std::cout << Cursor::Cursor::colorText(VERDE, msj);
    }
}

void NewProcesses::obtainField(std::string msj, std::string msjError,
                         Process& proc,
                         bool(Process::*metodo)(const std::string&))
{
    std::string aux;
    bool once = false;
    std::cout << Cursor::colorText(VERDE, msj);
    while(1) {
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((proc.*metodo)(aux))
            break;
        if (!once) {
            once = true;
            Cursor::rmLine();
        }
        else
            Cursor::rmLine(2);
        std::cout << Cursor::colorText(ROJO, msjError, true) << std::endl;
        std::cout << Cursor::colorText(VERDE, msj);
    }
}

///////////////////////////////////////////////
/*           EJECUCIÓN DE ProcessS           */
///////////////////////////////////////////////
void NewProcesses::executeProcess()
{
    unsigned int NewProcessessRes = this->pending.size() / MAX_JOB_AMOUNT;
    unsigned int NewProcessessTerm = 0;
    long cont = 0;
    bool jump;
    
    Cursor::clrscr();
    Frame pend(1, FRAME_Y, FIELD_WIDTH * 3 + 2, MAX_SIZE_JOBS_FRAME, AMARILLO);
    Frame actual(FIELD_WIDTH * 3 + 4, FRAME_Y, FIELD_WIDTH * 5, 6, VERDE);
    Frame term(FIELD_WIDTH * 8 + 5, FRAME_Y, FIELD_WIDTH * 5, 25, CYAN);
    Frame bloq(1, FRAME_Y + MAX_SIZE_JOBS_FRAME + 1, FIELD_WIDTH * 2 + 1,
              MAX_SIZE_JOBS_FRAME, MORADO);

    if (!(this->pending.size() % MAX_JOB_AMOUNT))
        NewProcessessRes--;
    printFrames(&pend, &actual, &term, &bloq);
    while (this->pending.size()) {
        jump = false;
        this->current = new Process(this->pending.front());
        this->pending.erase(this->pending.begin());

        pend.rmContent();
        printFrames(&pend);
        for (size_t i = 0; i < this->pending.size(); ++i)
                fillFrame(pend, this->pending[i], false, false);
        
        cont = this->current->getMaxTime()
               - this->current->getLapsedTime();
        while ((cont--) > 0) {
            jump = keyListener(cont);
            actual.rmContent();
            printFrames(nullptr, &actual);
            fillFrame(actual, *this->current, true, false);

            Cursor::gotoxy(1, 2);
            Cursor::rmLine(2);
            std::cout << "Processs nuevos: " << NewProcessessRes << std::endl
                      << "Tiempo transcurrido: " << NewProcesses::lapsedTime;
          
            std::cout.flush();
            if (jump)
                break;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++NewProcesses::lapsedTime;
            this->current->setRemTime(cont);
            this->current->setLapsedTime(
                                       this->current->getLapsedTime() + 1);
        }

        if (!jump || cont == -2) {
            if (cont != -2)
                this->current->calculate();
            fillFrame(term, *this->current, false, true);
            this->finished.push_back(*this->current);
            if (!(this->finished.size() % MAX_JOB_AMOUNT)){
                NewProcessessRes--;
                NewProcessessTerm++;
            }
        }
        delete this->current; this->current = nullptr;
    }
    pend.rmContent(); actual.rmContent();
    printFrames(&pend, &actual);
}

bool NewProcesses::keyListener(long &cont)
{
    unsigned char input;
    if (kbhit()) {
        input = getch();
        switch (input) {
        case 'i': case 'I':
            return inter(cont);
        case 'p': case 'P':
            pause();
            return false;
        case 'e': case 'E':
            cont = -2;
            this->current->setResult("ERROR");
            return true;
        default:
            return false;
        }
    }
    return false;
}

bool NewProcesses::inter(long &cont)
{
    std::vector<Process>::iterator it = this->pending.begin();
    //unsigned int NewProcesses = it->getNewProcesses();
    if ((this->finished.size() % MAX_JOB_AMOUNT) != MAX_JOB_AMOUNT - 1){
        cont = 0;
    //    for (it; it->getNewProcesses() == NewProcesses && it != this->pending.end(); it++);
        this->pending.insert(it, *this->current);
        return true;
    }
    return false;
}

void NewProcesses::pause()
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
void NewProcesses::fillFrame(Frame& marco, Process& proc, bool actual, bool term)
{
    marco.print(std::to_string(proc.getId()), BLANCO, false, FIELD_WIDTH);
    if (!actual && !term) {
        marco.print(std::to_string(proc.getMaxTime()), BLANCO, false,
                    FIELD_WIDTH);
        marco.print(std::to_string(proc.getLapsedTime()), BLANCO, true,
                    FIELD_WIDTH);
    }
    else if (actual || term) {
        marco.print(proc.getOp(), BLANCO, false, FIELD_WIDTH);
        marco.print(std::to_string(proc.getMaxTime()), BLANCO, false,
                    FIELD_WIDTH);
    }
    if (actual) {
        marco.print(std::to_string(proc.getRemTime()), BLANCO, false,
                    FIELD_WIDTH);
        marco.print(std::to_string(proc.getLapsedTime()), BLANCO, true,
                    FIELD_WIDTH);
    }
    else if (term)
        marco.print(proc.getResult(), BLANCO, false, FIELD_WIDTH);
}

void NewProcesses::printFrames(Frame* pend, Frame* act, Frame* term, Frame* bloq)
{
    if (pend) {
        pend->print("Processs pendientes:", BLANCO, true);
        pend->print("ID      TME     TMT", BLANCO, true);
    }
    if (act) {
        act->print("Processs actual:", BLANCO, true);
        act->print("ID      OP      TME     TMR     TMT",
                   BLANCO, true);
    }
    if (term){
        term->print("Processs terminados:", BLANCO, true);
        term->print("ID      OP      TME     RES", BLANCO, true);
    }
    if (bloq) {
        bloq->print("Processs bloqueados:", BLANCO, true);
        bloq->print("ID      TTB", BLANCO, true);
    }
}