#include "../define/processManager.h"

std::map<std::string, bool> ProcessManager::idsUsed;

ProcessManager::ProcessManager()
{
    srand(time(NULL));
    this->current = nullptr;
    this->lapsedTime = 0;
    this->lastId = 0;
    controller = Controller(&pending, &ready, &finished, &blocked, nullptr,
                            &states, &stateColors, &lapsedTime, &quantum);
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

void ProcessManager::printFinished()
{ controller.printBCP(true); }

void ProcessManager::init()
{
    unsigned long proc = 0;
    
    Cursor::clrscr();
    std::cout << Cursor::colorText(VERDE, "Procesos a capturar: ");
    std::cin >> proc;
    std::cout << Cursor::colorText(VERDE, "Quantum a utilizar: ");
    std::cin >> quantum;
    while (proc--)
        obtainProcess(++lastId);
    execute();
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
            controller.readyUp = true;
            blocked.erase(it);
        } else {
            it->setBlockedTime(it->getBlockedTime() - 1);
            it++;
        }
    }
    if (!blocked.size()) {
        controller.printFrames(false, false, false, true);
        controller.blockedUp = false;
    }
}

void ProcessManager::createDummyProcess(const long &execTime)
{
    current = new Process;
    current->setMaxTime(std::to_string(execTime));
    current->setId(std::to_string(0));
    current->setRemTime(execTime);
    controller.current = current;
    allBlocked = true;
    jump = CONTI;
}

void ProcessManager::executeProcess(long execTime)
{
    while (execTime--) {
        allBlocked = false;
        // Se asigna un tiempo de respuesta si aún no se ha asignado uno
        if (current->getResponseTime() == NO_RESPONSE_TIME)
            current->setResponseTime(lapsedTime - current->getArrivalTime());
        jump = keyListener(execTime);
        // Se genera proceso extra si ya hay 5 procesos bloqueados
        if (dummyProcess() && !allBlocked){
            execTime = blocked.begin()->getBlockedTime();
            createDummyProcess(execTime);
            execTime--;
        }
        controller.printUpdated();
        if (jump == INTER || jump == ERROR || (allBlocked && jump == NEWP && !pending.size()))
            break;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        checkBlocked();
        ++ProcessManager::lapsedTime;
        current->setRemTime(execTime);
        current->setServiceTime(current->getServiceTime() + 1);
        current->setQuantum(current->getQuantum() + 1);
        if (current->getQuantum() == quantum && current->getId()) {
            jump = QUANTUM;
            break;
        }
        controller.printUpdated();
    }
}

void ProcessManager::execute()
{
    controller.initFrames();
    while (processLeft()) {
        jump = CONTI;
        allBlocked = false;
        pendingToReady();
        if (ready.size()) {
            current = new Process(ready.front());
            controller.current = current;
            ready.erase(ready.begin());
        }
        else
            createDummyProcess(blocked.front().getBlockedTime());
        current->setQuantum(0);
        controller.readyUp = true;
        controller.printUpdated();
        executeProcess(current->getMaxTime() - current->getServiceTime());
        if (jump != INTER) {
            if (jump == QUANTUM && current->getRemTime())
                ready.push_back(*current);
            else if (current->getId()) {
                current->setFinishTime(lapsedTime);
                current->setReturnTime(current->getFinishTime() - current->getArrivalTime());
                current->setWaitingTime(current->getReturnTime() - current->getServiceTime());
                if (current->getResponseTime() == NO_RESPONSE_TIME)
                    current->setResponseTime(0);
                if (jump != ERROR)
                    current->calculate();
                controller.finishedUp = true;
                finished.push_back(*current);
            }
            delete current;
            controller.current = nullptr;
            current = nullptr;
        }
        controller.printUpdated();
    }
    controller.endFrames();
}

unsigned short ProcessManager::keyListener(long &cont)
{
    unsigned char input;
    if (kbhit()) {
        GETCH(input);
        switch (input) {
            case 'i': case 'I':
                controller.blockedUp = true;
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
                    controller.readyUp = true;
                }
                controller.printUpdated();
                return NEWP;
            case 'b': case 'B':
                pause(true);
                controller.redrawBCP();
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
    if (!pending.size() && !ready.size() && blocked.size() && current == nullptr)
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
        controller.current = nullptr;
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
        controller.printBCP(lapsedTime);
    } else {
        Cursor::gotoxy(1, 4);
        std::cout << Cursor::colorText(MORADO,
                            "Ejecucion pausada - Presione \"c\" para continuar");
    }    
    while (1) {
        if (kbhit()) {
            GETCH(input);
            if (input == 'c' || input == 'C') {
                Cursor::rmLine();
                if (bcp)
                    Cursor::clrscr();
                break;
            }
        }
    }
}
