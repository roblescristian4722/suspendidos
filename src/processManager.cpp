#include "../define/ProcessManager.h"

std::map<std::string, bool> ProcessManager::idsUsed;

ProcessManager::ProcessManager()
{
    srand(time(NULL));
    this->current = nullptr;
    this->lapsedTime = 0;
    this->lastId = 0;
    this->states[&finished] = "Finalizado";
    this->states[&ready] = "Listo";
    this->states[&blocked] = "Bloqueado";
    this->states[&pending] = "Nuevo";
    this->stateColors[&finished] = CYAN;
    this->stateColors[&ready] = VERDE;
    this->stateColors[&blocked] = ROJO;
    this->stateColors[&pending] = AMARILLO;
    this->readyF.setFrame(1, FRAME_Y, FIELD_WIDTH * 3 + 2,
                          MAX_SIZE_JOBS_FRAME + 2, AMARILLO);
    this->currentF.setFrame(FIELD_WIDTH * 3 + 4, FRAME_Y, FIELD_WIDTH * 5, 6,
                            VERDE);
    this->finishedF.setFrame(FIELD_WIDTH * 8 + 5, FRAME_Y, FIELD_WIDTH * 4, 25,
                             CYAN);
    this->blockedF.setFrame(1, FRAME_Y + MAX_SIZE_JOBS_FRAME + 2,
                            FIELD_WIDTH * 2 + 2, MAX_SIZE_JOBS_FRAME + 3, MORADO);
}

ProcessManager::~ProcessManager()
{ 
    if (current)
        delete current;
    current = nullptr;
}

const std::vector<Process>& ProcessManager::getReady() const
{ return this->pending; }

Process* ProcessManager::getCurrent() const
{ return this->current; }

const unsigned long& ProcessManager::getId() const
{ return this->id; }

void ProcessManager::printFinished()
{ printBCP(&finished); }

void ProcessManager::printBCP(bool finished)
{
    std::vector<Process>* queue;
    std::map<std::vector<Process>*, std::string>::iterator it;
    for (it = states.begin(); it != states.end(); it++) {
        queue = it->first;
        if (finished)
            queue = &this->finished;
        for (size_t i = 0; i < queue->size(); ++i) {
            std::cout << "ID: " << (*queue)[i].getId() << std::endl
                      << "Estado: " << Cursor::colorText(stateColors[queue],
                                                    states[queue]) << std::endl;
            if (states[queue] != "Nuevo") {
                if (states[queue] != "Finalizado")
                    (*queue)[i].setWaitingTime(lapsedTime - (*queue)[i].getArrivalTime()
                                               - (*queue)[i].getServiceTime());
                std::cout << "Operacion: " << (*queue)[i].getOp() << std::endl
                        << "Tiempo maximo estimado: " << (*queue)[i].getMaxTime()
                        << std::endl
                        << "Tiempo de llegada: " << (*queue)[i].getArrivalTime()
                        << std::endl
                        << "Tiempo de servicio: " << (*queue)[i].getServiceTime()
                        << std::endl;
                std::cout << "Tiempo de espera: " << (*queue)[i].getWaitingTime()
                          << std::endl;
                if ((*queue)[i].getResponseTime() != NO_RESPONSE_TIME)
                    std::cout << "Tiempo de respuesta: " << (*queue)[i].getResponseTime()
                            << std::endl;
                if (states[queue] == "Finalizado")
                    std::cout << "Resultado: " << (*queue)[i].getResult() << std::endl
                            << "Tiempo de finalizacion: " << (*queue)[i].getFinishTime()
                            << std::endl
                            << "Tiempo de retorno: " << (*queue)[i].getReturnTime()
                            << std::endl;
                if (states[queue] == "Bloqueado")
                    std::cout << "Tiempo bloqueado restante: "
                              << (*queue)[i].getBlockedTime() << std::endl;
            }
            std::cout << std::endl;
        }
        if (finished)
            break;
    }   
    if (current != nullptr) {
        if (current->getId()) {
            std::cout << "ID: " << current->getId() << std::endl
                    << "Estado:" << Cursor::colorText(MORADO, "En ejecucion")
                    << std::endl
                    << "Operacion: " << current->getOp() << std::endl
                    << "Tiempo maximo estimado: " << current->getMaxTime()
                    << std::endl
                    << "Tiempo restante: " << current->getRemTime()
                    << std::endl
                    << "Tiempo de llegada: " << current->getArrivalTime()
                    << std::endl;
            current->setWaitingTime(lapsedTime - current->getArrivalTime()
                                    - current->getServiceTime());
            std::cout << "Tiempo de espera: " << current->getWaitingTime()
                    << std::endl
                    << "Tiempo de servicio: " << current->getServiceTime()
                    << std::endl
                    << "Tiempo de respuesta: " << current->getResponseTime()
                    << std::endl << std::endl;
        }
    }
}

void ProcessManager::setId(const unsigned long &id)
{ this->id = id; }

void ProcessManager::init()
{
    unsigned long proc = 0;
    
    Cursor::clrscr();
    std::cout << Cursor::colorText(VERDE, "Procesos a capturar: ");
    std::cin >> proc;
    while (proc--)
        obtainProcess(++lastId);
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
    pending.push_back(aux);
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
    size_t tmp = current != nullptr ? 1 : 0;
    if (tmp)
        if (!current->getId())
            tmp--;
    return tmp + blocked.size() + ready.size();
}

void ProcessManager::pendingToReady()
{
    for (size_t i = 0; pending.size() && onMemory() <= MAX_READY_JOB_AMOUNT; ++i) {
        ready.push_back(*pending.begin());
        ready.back().setArrivalTime(lapsedTime);
        pending.erase(pending.begin());
    }
}

bool ProcessManager::processLeft()
{
    if (!blocked.size() && !pending.size() && !ready.size()
    && current == nullptr)
        return false;
    return true;
}

void ProcessManager::checkBlocked()
{
    std::vector<Process>::iterator it = blocked.begin();
    while (it < blocked.end()) {
        if (it->getBlockedTime() == 1) {
            ready.push_back(*it);
            fillReady(*it);
            blocked.erase(it);
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

    Cursor::hideCursor();
    Cursor::clrscr();
    readyF.drawFrame();
    blockedF.drawFrame();
    finishedF.drawFrame();
    currentF.drawFrame();
    printFrames(true, true, true, true);
    while (processLeft()) {
        jump = false;
        pendingToReady();
        current = new Process(ready.front());
        ready.erase(ready.begin());

        readyF.rmContent();
        printFrames(true);
        for (size_t i = 0; i < ready.size(); ++i)
            fillReady(ready[i]);
        
        cont = current->getMaxTime() - current->getServiceTime();
        allBlocked = false;
        while (cont--) {
            if (current->getResponseTime() == NO_RESPONSE_TIME)
                current->setResponseTime(lapsedTime
                - current->getArrivalTime());
            jump = keyListener(cont);
            if (jump == BCP) {
                currentF.drawFrame();
                blockedF.drawFrame();
                reDrawReady();
                reDrawFinished();
            }
            // Se genera proceso extra si ya hay 5 procesos bloqueados
            if (dummyProcess() && !allBlocked){
                cont = blocked.begin()->getBlockedTime();
                current = new Process;
                current->setMaxTime(std::to_string(cont));
                current->setId(std::to_string(0));
                current->setRemTime(cont);
                allBlocked = true;
                cont--;
                jump = CONTI;
            }
            printFrames(false, false, false, true);
            fillBlocked();
            currentF.rmContent();
            if (jump != ERROR && jump != INTER) {
                printFrames(false, true);
                fillCurrent();
            }
            auxStr = "Procesos nuevos: " + std::to_string(pending.size())
            + "\nTiempo transcurrido: " + std::to_string(ProcessManager::lapsedTime);
            Cursor::rmPrint(1, 2, auxStr);
            if (jump == INTER || jump == ERROR || (allBlocked && jump == NEWP && !pending.size()))
                break;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            checkBlocked();
            ++ProcessManager::lapsedTime;
            current->setRemTime(cont);
            current->setServiceTime(current->getServiceTime() + 1);
        }
        if (jump != INTER) {
            if (current->getId()) {
                current->setFinishTime(lapsedTime);
                current->setReturnTime(current->getFinishTime()
                - current->getArrivalTime());
                current->setWaitingTime(current->getReturnTime()
                - current->getServiceTime());
                if (current->getResponseTime() == NO_RESPONSE_TIME)
                    current->setResponseTime(0);
                if (jump != ERROR)
                    current->calculate();
                fillFinished(*current);
                finished.push_back(*current);
            }
            delete current;
            current = nullptr;
        }
    }
    readyF.rmContent();
    currentF.rmContent();
    printFrames(true, true);
    Cursor::showCursor();
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
            if (current->getId()) {
                current->setResult("ERROR");
                return ERROR;
            }
            return CONTI;
        case 'n': case 'N':
            obtainProcess(++lastId);
            if (onMemory() <= MAX_READY_JOB_AMOUNT) {
                ready.push_back(*pending.begin());
                ready.back().setArrivalTime(lapsedTime);
                pending.erase(pending.begin());
                reDrawReady();
            }
            return NEWP;
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
    if (blocked.size() == MAX_READY_JOB_AMOUNT + 1)
        return true;
    if (!pending.size() && !ready.size() && blocked.size()
    && current == nullptr)
        return true;
    return false;
}

unsigned short ProcessManager::inter(long &cont)
{
    if (blocked.size() <= MAX_READY_JOB_AMOUNT && current->getId()){
        current->setBlockedTime(MAX_BLOCKED_TIME);
        blocked.push_back(*current);
        delete current;
        current = nullptr;
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

void ProcessManager::fillBlocked()
{
    for (size_t i = 0; i < blocked.size(); ++i) {
        blockedF.print(std::to_string(blocked[i].getId()), BLANCO, false,
                FIELD_WIDTH);
        blockedF.print(std::to_string(blocked[i].getBlockedTime()), BLANCO, true,
                FIELD_WIDTH);
    }
}

void ProcessManager::reDrawFinished() {
    finishedF.drawFrame();
    printFrames(false, false, true);
    for (size_t i = 0; i < finished.size(); ++i)
        fillFinished(finished[i]);
}

void ProcessManager::reDrawReady() {
    readyF.drawFrame();
    printFrames(true);
    for (size_t i = 0; i < ready.size(); ++i)
        fillReady(ready[i]);
}

void ProcessManager::fillCurrent()
{
    currentF.print(std::to_string(current->getId()), BLANCO, false, FIELD_WIDTH);
    currentF.print(current->getOp(), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(current->getMaxTime()), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(current->getRemTime()), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(current->getServiceTime()), BLANCO, true, FIELD_WIDTH);
}

void ProcessManager::fillFinished(Process &p)
{
    finishedF.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    finishedF.print(p.getOp(), BLANCO, false, FIELD_WIDTH);
    finishedF.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    finishedF.print(p.getResult(), BLANCO, true, FIELD_WIDTH);
}

void ProcessManager::fillReady(Process &p)
{
    readyF.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    readyF.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    readyF.print(std::to_string(p.getServiceTime()), BLANCO, true, FIELD_WIDTH);
}

void ProcessManager::printFrames(bool rdy, bool act, bool fnshd, bool bloq)
{
    if (rdy) {
        readyF.print("Procesos listos:", BLANCO, true);
        readyF.print("ID      TME     TMT", BLANCO, true);
    }
    if (act) {
        currentF.update("Proceso en ejecucion:", BLANCO, true);
        currentF.print("ID      OP      TME     TMR     TMT",
                       BLANCO, true);
    }
    if (fnshd){
        finishedF.print("Procesos terminados:", BLANCO, true);
        finishedF.print("ID      OP      TME     RES", BLANCO, true);
    }
    if (bloq) {
        blockedF.update("Procesos bloqueados:", BLANCO, true);
        blockedF.print("ID      TRB", BLANCO, true);
    }
}