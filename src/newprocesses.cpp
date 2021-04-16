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
    this->pending = NewProcesses.ready;
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
    this->pending = NewProcesses.ready;
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

const std::vector<Process>& NewProcesses::getReady() const
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

void NewProcesses::obtainProcess(const unsigned long& cont)
{
    Process aux;
    unsigned int NewProcesses = cont / MAX_READY_JOB_AMOUNT;
    
    // Captura de ID
    aux.setId(std::to_string(cont));
    // Captura de operación
    aux.setOp(generateOp());
    // Captura de tiempo máximo
    aux.setMaxTime(std::to_string(generateTime()));
    this->pending.push_back(aux);
}

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

size_t NewProcesses::onMemory()
{
    size_t tmp = this->current != nullptr ? 1 : 0;
    if (tmp)
        if (!this->current->getId())
            tmp--;
    return tmp + this->blocked.size() + this->ready.size();
}

void NewProcesses::pendingToReady()
{
    for (size_t i = 0; this->pending.size() && onMemory() <= MAX_READY_JOB_AMOUNT; ++i) {
        this->ready.push_back(*this->pending.begin());
        this->pending.erase(this->pending.begin());
    }
}

bool NewProcesses::processLeft()
{
    if (!this->blocked.size() && !this->pending.size() && !this->ready.size()
    && this->current == nullptr)
        return false;
    return true;
}

void NewProcesses::checkBlocked(Frame &f)
{
    std::vector<Process>::iterator it = this->blocked.begin();
    while (it < this->blocked.end()) {
        if (it->getBlockedTime() == 1) {
            this->ready.push_back(*it);
            fillReady(f, *it);
            this->blocked.erase(it);
        } else {
            it->setBlockedTime(it->getBlockedTime() - 1);
            it++;
        }
    }
}

void NewProcesses::executeProcess()
{
    unsigned int readyProcesses = (this->pending.size() <= MAX_READY_JOB_AMOUNT)
                        ? 0 : this->pending.size() - MAX_READY_JOB_AMOUNT - 1;
    long cont;
    unsigned short jump;
    bool allBlocked;
    Cursor::clrscr();
    Frame rdy(1, FRAME_Y, FIELD_WIDTH * 3 + 2, MAX_SIZE_JOBS_FRAME + 2, AMARILLO);
    Frame crrnt(FIELD_WIDTH * 3 + 4, FRAME_Y, FIELD_WIDTH * 5, 6, VERDE);
    Frame fnshd(FIELD_WIDTH * 8 + 5, FRAME_Y, FIELD_WIDTH * 4, 25, CYAN);
    Frame bloq(1, FRAME_Y + MAX_SIZE_JOBS_FRAME + 2, FIELD_WIDTH * 2 + 2,
              MAX_SIZE_JOBS_FRAME + 3, MORADO);

    printFrames(&rdy, &crrnt, &fnshd, &bloq);
    while (processLeft()) {
        jump = false;
        pendingToReady();
        this->current = new Process(this->ready.front());
        this->ready.erase(this->ready.begin());

        rdy.rmContent();
        printFrames(&rdy);
        for (size_t i = 0; i < this->ready.size(); ++i)
            fillReady(rdy, this->ready[i]);
        
        cont = this->current->getMaxTime() - this->current->getLapsedTime();
        allBlocked = false;
        while (cont--) {
            jump = keyListener(cont);
            // Se genera proceso extra si ya hay 5 procesos bloqueados
            if (dummyProcess() && !allBlocked){
                cont = this->blocked.begin()->getBlockedTime();
                this->current = new Process;
                this->current->setMaxTime(std::to_string(cont));
                this->current->setId(std::to_string(0));
                this->current->setRemTime(cont);
                allBlocked = true;
                cont--;
                jump = 0;
            }
            printFrames(nullptr, nullptr, nullptr, &bloq);
            fillBlocked(bloq);
            crrnt.rmContent();
            if (!jump) {
                printFrames(nullptr, &crrnt);
                fillCurrent(crrnt, *this->current);
            }
            Cursor::gotoxy(1, 2);
            Cursor::rmLine(2);
            std::cout << "Procesos nuevos: " << readyProcesses << std::endl
                      << "Tiempo transcurrido: " << NewProcesses::lapsedTime;
            std::cout.flush();
          
            if (jump)
                break;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            checkBlocked(rdy);
            ++NewProcesses::lapsedTime;
            this->current->setRemTime(cont);
            this->current->setLapsedTime(this->current->getLapsedTime() + 1);
        }
        if (this->current != nullptr) {
            if (this->current->getId()) {
                if (jump != 2)
                    this->current->calculate();
                fillFinished(fnshd, *this->current);
                this->finished.push_back(*this->current);
                if (readyProcesses)
                    readyProcesses--;
            }
            delete this->current;
            this->current = nullptr;
        }
    }
    rdy.rmContent();
    crrnt.rmContent();
    printFrames(&rdy, &crrnt);
}

unsigned short NewProcesses::keyListener(long &cont)
{
    unsigned char input;
    if (kbhit()) {
        input = getch();
        switch (input) {
        case 'i': case 'I':
            return inter(cont);
        case 'p': case 'P':
            pause();
            return 0;
        case 'e': case 'E':
            if (this->current->getId()) {
                this->current->setResult("ERROR");
                return 2;
            }
            return 0;
        default:
            return 0;
        }
    }
    return 0;
}

bool NewProcesses::dummyProcess()
{
    if (this->blocked.size() == MAX_READY_JOB_AMOUNT + 1)
        return true;
    if (!this->pending.size() && !this->ready.size() && this->blocked.size()
    && this->current == nullptr)
        return true;
    return false;
}

unsigned short NewProcesses::inter(long &cont)
{
    if (this->blocked.size() <= MAX_READY_JOB_AMOUNT && this->current->getId()){
        this->current->setBlockedTime(MAX_BLOCKED_TIME);
        this->blocked.push_back(*this->current);
        delete this->current;
        this->current = nullptr;
        return 1;
    }
    return 0;
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

void NewProcesses::fillBlocked(Frame &f)
{
    for (size_t i = 0; i < this->blocked.size(); ++i) {
        f.print(std::to_string(this->blocked[i].getId()), BLANCO, false,
                FIELD_WIDTH);
        f.print(std::to_string(this->blocked[i].getBlockedTime()), BLANCO, true,
                FIELD_WIDTH);
    }
}

void NewProcesses::fillCurrent(Frame &f, Process &p)
{
    f.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    f.print(p.getOp(), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getRemTime()), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getLapsedTime()), BLANCO, true, FIELD_WIDTH);
}

void NewProcesses::fillFinished(Frame &f, Process &p)
{
    f.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    f.print(p.getOp(), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    f.print(p.getResult(), BLANCO, true, FIELD_WIDTH);
}

void NewProcesses::fillReady(Frame &f, Process &p)
{
    f.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getLapsedTime()), BLANCO, true, FIELD_WIDTH);
}

void NewProcesses::printFrames(Frame* rdy, Frame* act, Frame* fnshd, Frame* bloq)
{
    if (rdy) {
        rdy->print("Procesos pendientes:", BLANCO, true);
        rdy->print("ID      TME     TMT", BLANCO, true);
    }
    if (act) {
        act->update("Proceso en ejecucion:", BLANCO, true);
        act->print("ID      OP      TME     TMR     TMT",
                   BLANCO, true);
    }
    if (fnshd){
        fnshd->print("Procesos terminados:", BLANCO, true);
        fnshd->print("ID      OP      TME     RES", BLANCO, true);
    }
    if (bloq) {
        bloq->update("Procesos bloqueados:", BLANCO, true);
        bloq->print("ID      TTB", BLANCO, true);
    }
}