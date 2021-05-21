#include "../define/ProcessManager.h"

std::map<std::string, bool> ProcessManager::idsUsed;

ProcessManager::ProcessManager()
{
    srand(time(NULL));
    this->current = nullptr;
    this->lapsedTime = 0;
    this->lastId = 0;
    controller.initStateColors(this);
    controller.initStates(this);
    controller.initFrames(this);
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
{ controller.printBCP(this, true); }

void ProcessManager::setId(const unsigned long &id)
{ this->id = id; }

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
            controller.fillReady(this,*it);
            blocked.erase(it);
        } else {
            it->setBlockedTime(it->getBlockedTime() - 1);
            it++;
        }
    }
}

void ProcessManager::createDummyProcess(const long &execTime)
{
    current = new Process;
    current->setMaxTime(std::to_string(execTime));
    current->setId(std::to_string(0));
    current->setRemTime(execTime);
    allBlocked = true;
    jump = CONTI;
}

void ProcessManager::executeProcess(long execTime)
{
    std::string auxStr = "";
    while (execTime--) {
        allBlocked = false;
        // Se asigna un tiempo de respuesta si aún no se ha asignado uno
        if (current->getResponseTime() == NO_RESPONSE_TIME)
            current->setResponseTime(lapsedTime - current->getArrivalTime());
        jump = keyListener(execTime);
        if (jump == BCP) {
            currentF.drawFrame();
            blockedF.drawFrame();
            controller.reDrawReady(this);
            controller.reDrawFinished(this);
        }
        // Se genera proceso extra si ya hay 5 procesos bloqueados
        if (dummyProcess() && !allBlocked){
            execTime = blocked.begin()->getBlockedTime();
            createDummyProcess(execTime);
            execTime--;
        }

        controller.printFrames(this, false, false, false, true);
        controller.fillBlocked(this);
        currentF.rmContent();

        if (jump != ERROR && jump != INTER) {
            controller.printFrames(this, false, true);
            controller.fillCurrent(this);
        }

        auxStr = "Procesos nuevos: " + std::to_string(pending.size())
        + "\nTiempo transcurrido: " + std::to_string(ProcessManager::lapsedTime)
        + "\nValor del Quantum: " + std::to_string(quantum);
        Cursor::rmPrint(1, 2, auxStr);

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
    }
}

void ProcessManager::execute()
{
    bool allBlocked;
    Cursor::hideCursor();
    Cursor::clrscr();
    readyF.drawFrame();
    blockedF.drawFrame();
    finishedF.drawFrame();
    currentF.drawFrame();
    controller.printFrames(this, true, true, true, true);
    while (processLeft()) {
        jump = CONTI;
        allBlocked = false;

        pendingToReady();
        if (ready.size()) {
            current = new Process(ready.front());
            ready.erase(ready.begin());
        }
        else
            createDummyProcess(blocked.front().getBlockedTime());
        current->setQuantum(0);

        readyF.rmContent();
        controller.printFrames(this, true);
        for (size_t i = 0; i < ready.size(); ++i)
            controller.fillReady(this,ready[i]);
            
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
                controller.fillFinished(this, *current);
                finished.push_back(*current);
            }
            delete current;
            current = nullptr;
        }
    }
    readyF.rmContent();
    currentF.rmContent();
    controller.printFrames(this, true, true);
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
                    controller.reDrawReady(this);
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
        controller.printBCP(this);
    } else {
        Cursor::gotoxy(1, 4);
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