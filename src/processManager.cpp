#include "../define/ProcessManager.h"

std::map<std::string, bool> ProcessManager::idsUsed;

ProcessManager::ProcessManager()
{
    srand(time(NULL));
    this->current = nullptr;
    this->lapsedTime = 0;
    this->states[&this->finished] = "Finalizado";
    this->states[&this->ready] = "Listo";
    this->states[&this->blocked] = "Bloqueado";
    this->states[&this->pending] = "Nuevo";
    this->stateColors[&this->finished] = CYAN;
    this->stateColors[&this->ready] = VERDE;
    this->stateColors[&this->blocked] = ROJO;
    this->stateColors[&this->pending] = AMARILLO;
}

ProcessManager::~ProcessManager()
{ 
    if (this->current)
        delete this->current;
    this->current = nullptr;
}

const std::vector<Process>& ProcessManager::getReady() const
{ return this->pending; }

Process* ProcessManager::getCurrent() const
{ return this->current; }

const unsigned long& ProcessManager::getId() const
{ return this->id; }

void ProcessManager::printFinished()
{ printBCP(&this->finished); }

bool ProcessManager::printBCPData(std::vector<Process> *v)
{
    if (this->states[v] == "Finalizado")
        return true;
    return false;
}

void ProcessManager::printBCP(const bool& finished)
{
    std::vector<Process>* queue;
    std::map<std::vector<Process>*, std::string>::iterator it;
    for (it = this->states.begin(); it != this->states.end(); it++) {
        queue = it->first;
        if (finished)
            queue = &this->finished;
        for (size_t i = 0; i < queue->size(); ++i) {
            std::cout << "ID: " << (*queue)[i].getId() << std::endl
                    << "Estado: " << Cursor::colorText(this->stateColors[queue],
                        this->states[queue]) << std::endl
                    << "Operacion: " << (*queue)[i].getOp() << std::endl
                    << "Tiempo maximo estimado: " << (*queue)[i].getMaxTime()
                    << std::endl
                    << "Tiempo de llegada: " << (*queue)[i].getArrivalTime()
                    << std::endl
                    << "Tiempo de espera: " << (*queue)[i].getWaitingTime()
                    << std::endl
                    << "Tiempo de servicio: " << (*queue)[i].getServiceTime()
                    << std::endl;
            if (this->states[queue] == "Finalizado")
                std::cout << "Resultado: " << (*queue)[i].getResult() << std::endl
                        << "Tiempo de finalizacion: " << (*queue)[i].getFinishTime()
                        << std::endl
                        << "Tiempo de retorno: " << (*queue)[i].getReturnTime()
                        << std::endl
                        << "Tiempo de respuesta: " << (*queue)[i].getResponseTime()
                        << std::endl;
            std::cout << std::endl;
            if (finished)
                break;
        }
    }
    if (this->current != nullptr)
        if (this->current->getId())
            std::cout << "ID: " << this->current->getId() << std::endl
                    << "Estado:" << Cursor::colorText(MORADO, "En ejecucion")
                    << std::endl
                    << "Operacion: " << this->current->getOp() << std::endl
                    << "Tiempo maximo estimado: " << this->current->getMaxTime()
                    << std::endl
                    << "Tiempo restante:" << this->current->getRemTime()
                    << std::endl
                    << "Tiempo de llegada: " << this->current->getArrivalTime()
                    << std::endl
                    << "Tiempo de espera: " << this->current->getWaitingTime()
                    << std::endl
                    << "Tiempo de servicio: " << this->current->getServiceTime()
                    << std::endl
                    << "Tiempo de respuesta: " << this->current->getResponseTime()
                    << std::endl << std::endl;
}

void ProcessManager::setId(const unsigned long &id)
{ this->id = id; }

void ProcessManager::init()
{
    unsigned long proc = 0;
    unsigned long cont = 0;
    
    std::cout << Cursor::colorText(VERDE, "Procesos a capturar: ");
    std::cin >> proc;
    while (proc--)
        obtainProcess(++cont);
    executeProcess();
}

void ProcessManager::obtainProcess(const unsigned long& cont)
{
    Process aux;    
    // Captura de ID
    aux.setId(std::to_string(cont));
    // Captura de operación
    aux.setOp(generateOp());
    // Captura de tiempo máximo
    aux.setMaxTime(std::to_string(generateTime()));
    this->pending.push_back(aux);
}

std::string ProcessManager::generateOp()
{   
    const short numOp = 5;
    char op[numOp] = { '+', '-', '*', '/', '%' };
    std::string tmp = "";

    tmp = std::to_string((rand() % 99) + 1);
    tmp += op[rand() % numOp];
    tmp += std::to_string((rand() % 99) + 1);
    return tmp;
}

unsigned long ProcessManager::generateTime()
{
    const short maxTmp = 10;
    unsigned long tmp = 1;
    tmp = (rand() % maxTmp) + 6;
    return tmp;
}

void ProcessManager::obtainField(std::string msj, std::string msjError,
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

void ProcessManager::obtainField(std::string msj, std::string msjError,
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

size_t ProcessManager::onMemory()
{
    size_t tmp = this->current != nullptr ? 1 : 0;
    if (tmp)
        if (!this->current->getId())
            tmp--;
    return tmp + this->blocked.size() + this->ready.size();
}

void ProcessManager::pendingToReady()
{
    for (size_t i = 0; this->pending.size() && onMemory() <= MAX_READY_JOB_AMOUNT; ++i) {
        this->ready.push_back(*this->pending.begin());
        this->ready.back().setArrivalTime(this->lapsedTime);
        this->pending.erase(this->pending.begin());
    }
}

bool ProcessManager::processLeft()
{
    if (!this->blocked.size() && !this->pending.size() && !this->ready.size()
    && this->current == nullptr)
        return false;
    return true;
}

void ProcessManager::checkBlocked(Frame &f)
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

void ProcessManager::executeProcess()
{
    std::string auxStr = "";
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
        
        cont = this->current->getMaxTime() - this->current->getServiceTime();
        allBlocked = false;
        while (cont--) {
            if (this->current->getResponseTime() == NO_RESPOND_TIME)
                this->current->setResponseTime(this->lapsedTime
                - this->current->getArrivalTime());
            jump = keyListener(cont);
            if (jump == BCP) {
                crrnt.drawFrame();
                bloq.drawFrame();
                reDrawReady(rdy);
                reDrawFinished(fnshd);
            }
            // Se genera proceso extra si ya hay 5 procesos bloqueados
            if (dummyProcess() && !allBlocked){
                cont = this->blocked.begin()->getBlockedTime();
                this->current = new Process;
                this->current->setMaxTime(std::to_string(cont));
                this->current->setId(std::to_string(0));
                this->current->setRemTime(cont);
                allBlocked = true;
                cont--;
                jump = CONTI;
            }
            printFrames(nullptr, nullptr, nullptr, &bloq);
            fillBlocked(bloq);
            crrnt.rmContent();
            if (jump == CONTI || jump == BCP) {
                printFrames(nullptr, &crrnt);
                fillCurrent(crrnt, *this->current);
            }
            auxStr = "Procesos nuevos: " + std::to_string(this->pending.size())
            + "\nTiempo transcurrido: " + std::to_string(ProcessManager::lapsedTime);
            Cursor::rmPrint(1, 2, auxStr);
            if (jump != CONTI && jump != BCP)
                break;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            checkBlocked(rdy);
            ++ProcessManager::lapsedTime;
            this->current->setRemTime(cont);
            this->current->setServiceTime(this->current->getServiceTime() + 1);
        }
        if (jump != INTER) {
            if (this->current->getId()) {
                this->current->setFinishTime(this->lapsedTime);
                this->current->setReturnTime(this->current->getFinishTime()
                - this->current->getArrivalTime());
                this->current->setWaitingTime(this->current->getReturnTime()
                - this->current->getServiceTime());
                if (this->current->getResponseTime() == NO_RESPOND_TIME)
                    this->current->setResponseTime(0);
                if (jump != ERROR)
                    this->current->calculate();
                fillFinished(fnshd, *this->current);
                this->finished.push_back(*this->current);
            }
            delete this->current;
            this->current = nullptr;
        }
    }
    rdy.rmContent();
    crrnt.rmContent();
    printFrames(&rdy, &crrnt);
}

unsigned short ProcessManager::keyListener(long &cont)
{
    unsigned char input;
    if (kbhit()) {
        input = getch();
        switch (input) {
        case 'i': case 'I':
        return inter(cont);
        case 'p': case 'P':
            pause();
        return CONTI;
        case 'e': case 'E':
            if (this->current->getId()) {
                this->current->setResult("ERROR");
                return ERROR;
            }
        return CONTI;
        case 'n': case 'N':
            obtainProcess(this->pending.back().getId() + 1);
        break;
        case 'b': case 'B':
            pause(true);
        return BCP;
        default:
        return CONTI;
        }
    }
    return CONTI;
}

bool ProcessManager::dummyProcess()
{
    if (this->blocked.size() == MAX_READY_JOB_AMOUNT + 1)
        return true;
    if (!this->pending.size() && !this->ready.size() && this->blocked.size()
    && this->current == nullptr)
        return true;
    return false;
}

unsigned short ProcessManager::inter(long &cont)
{
    if (this->blocked.size() <= MAX_READY_JOB_AMOUNT && this->current->getId()){
        this->current->setBlockedTime(MAX_BLOCKED_TIME);
        this->blocked.push_back(*this->current);
        delete this->current;
        this->current = nullptr;
        return INTER;
    }
    return CONTI;
}

void ProcessManager::pause(const bool& bcp)
{
    unsigned char input;
    if (bcp) {
        Cursor::clrscr();
        std::cout << Cursor::colorText(MORADO,
                        "Ejecucion pausada - Presione \"c\" para continuar")
                  << std::endl;
        printBCP();
    } else {
        Cursor::gotoxy(1, 3);
        std::cout << Cursor::colorText(MORADO,
                            "Ejecucion pausada - Presione \"c\" para continuar");
    }    
    while (1) {
        if (kbhit()) {
            input = getch();
            if (input == 'c' || input == 'C') {
                Cursor::rmLine();
                if (bcp)
                    Cursor::clrscr();
                break;
            }
        }
    }
}

void ProcessManager::fillBlocked(Frame &f)
{
    for (size_t i = 0; i < this->blocked.size(); ++i) {
        f.print(std::to_string(this->blocked[i].getId()), BLANCO, false,
                FIELD_WIDTH);
        f.print(std::to_string(this->blocked[i].getBlockedTime()), BLANCO, true,
                FIELD_WIDTH);
    }
}

void ProcessManager::reDrawFinished(Frame &f) {
    f.drawFrame();
    printFrames(nullptr, nullptr, &f);
    for (size_t i = 0; i < this->finished.size(); ++i)
        fillFinished(f, this->finished[i]);
}

void ProcessManager::reDrawReady(Frame &f) {
    f.drawFrame();
    printFrames(&f);
    for (size_t i = 0; i < this->ready.size(); ++i)
        fillReady(f, this->ready[i]);
}

void ProcessManager::fillCurrent(Frame &f, Process &p)
{
    f.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    f.print(p.getOp(), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getRemTime()), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getServiceTime()), BLANCO, true, FIELD_WIDTH);
}

void ProcessManager::fillFinished(Frame &f, Process &p)
{
    f.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    f.print(p.getOp(), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    f.print(p.getResult(), BLANCO, true, FIELD_WIDTH);
}

void ProcessManager::fillReady(Frame &f, Process &p)
{
    f.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    f.print(std::to_string(p.getServiceTime()), BLANCO, true, FIELD_WIDTH);
}

void ProcessManager::printFrames(Frame* rdy, Frame* act, Frame* fnshd, Frame* bloq)
{
    if (rdy) {
        rdy->print("Procesos listos:", BLANCO, true);
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