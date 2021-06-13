#include "../define/processManager.h"

std::map<std::string, bool> ProcessManager::idsUsed;

Page::Page(const short &id, std::vector<Process> *state, const short &size,
           bool working)
{
        this->id = id;
        this->state = state;
        this->size = size;
        this->working = working;
}

ProcessManager::ProcessManager()
{
    srand(time(NULL));
    this->current = nullptr;
    this->lapsedTime = 0;
    this->lastId = 0;
    this->controller = Controller(&pending, &ready, &finished, &blocked, nullptr,
                            &lapsedTime, &quantum, &memory);
    this->emptyFrames = MEMORY_PARTITIONS;
    for (short i = MEMORY_PARTITIONS - 1; i >= MEMORY_PARTITIONS - SO_PAGES; --i) {
        this->memory[i].id = -1;
        this->memory[i].size = PARTITION_SIZE;
    }
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
    // Captura del tamaño del proceso
    aux.setSize(generateSize());
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
    return (rand() % maxTmp) + 6;
}

short ProcessManager::generateSize()
{ return (rand() % (MAX_PAGE_SIZE - MIN_PAGE_SIZE + 1)) + MIN_PAGE_SIZE; }

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
    bool exit = false;
    while (it < blocked.end()) {
        if (it->getBlockedTime() == 1) {
            ready.push_back(*it);
            updatePage(ready.back().getId(), &ready, false, exit, &ready.back());
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

bool ProcessManager::dummyProcess()
{
    if (blocked.size() && !ready.size() && current == nullptr)
        return true;
    /* if (!pending.size() && !ready.size() && blocked.size() && current == nullptr) */
    /*     return true; */
    return false;
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
    bool exit = false;
    controller.initFrames();
    while (processLeft()) {
        jump = CONTI;
        allBlocked = false;
        while(pushToMemory());
        if (ready.size()) {
            current = new Process(ready.front());
            controller.current = current;
            ready.erase(ready.begin());
            updatePage(current->getId(), &ready, true, exit, current);
        }
        else
            createDummyProcess(blocked.front().getBlockedTime());
        current->setQuantum(0);
        controller.readyUp = true;
        controller.memoryUp = true;
        controller.printUpdated();
        allBlocked = false;
        executeProcess(current->getMaxTime() - current->getServiceTime());
        if (jump != INTER) {
            if (jump == QUANTUM && current->getRemTime()){
                ready.push_back(*current);
                updatePage(ready.back().getId(), &ready, false, exit, &ready.back());
            }
            else if (current->getId()) {
                current->setFinishTime(lapsedTime);
                current->setReturnTime(current->getFinishTime() - current->getArrivalTime());
                current->setWaitingTime(current->getReturnTime() - current->getServiceTime());
                if (current->getResponseTime() == NO_RESPONSE_TIME)
                    current->setResponseTime(0);
                if (jump != ERROR)
                    current->calculate();
                updatePage(0, nullptr, false, exit, current, true);
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
            case 'p': case 'P': case 'a': case 'A':
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
                if (pushToMemory())
                    controller.readyUp = true;
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

unsigned short ProcessManager::inter(long &cont)
{
    bool exit = false;
    if (current->getId()){
        current->setBlockedTime(MAX_BLOCKED_TIME);
        blocked.push_back(*current);
        updatePage(blocked.back().getId(), &blocked, false, exit, &blocked.back());
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
    while (true) {
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

bool ProcessManager::pushToMemory()
{
    short size = pending.front().getSize();
    short pages = size / PARTITION_SIZE;
    std::vector<short> frames;
    (size % PARTITION_SIZE) ? pages++ : pages;
    if (pages <= emptyFrames && pages && pending.size()) {
        for (short i = 0; i < MEMORY_PARTITIONS; ++i)
            if (!memory[i].id) {
                memory[i] = Page(pending.front().getId(), &ready,
                        (size >= PARTITION_SIZE) ? PARTITION_SIZE : size, false);
                size >= PARTITION_SIZE ? size -= PARTITION_SIZE : size = 0;
                emptyFrames--;
                if (!(--pages))
                    break;
            }
        ready.push_back(*pending.begin());
        ready.back().setArrivalTime(lapsedTime);
        ready.back().setFrame(frames);
        pending.erase(pending.begin());
        return true;
    }
    return false;
}

void ProcessManager::updatePage(const short &id, std::vector<Process> *s,
                                bool w, bool &e, const Process *p, bool freeMemory)
{
    for (short i = 0; i < MEMORY_PARTITIONS; ++i) {
        if (p != nullptr) {
            if (memory[i].id == p->getId()){
                memory[i].id = id;
                memory[i].state = s;
                memory[i].working = w;
                e = true;
                if (freeMemory)
                    emptyFrames++;
            }
            else if (e)
                break;
        }
    }
    e = false;
}
