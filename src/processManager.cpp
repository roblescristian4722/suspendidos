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
    this->controller = Controller(this);
    this->emptyFrames = MEMORY_PARTITIONS - SO_PAGES;
    for (short i = MEMORY_PARTITIONS - 1; i >= MEMORY_PARTITIONS - SO_PAGES; --i) {
        this->memory[i].id = -1;
        this->memory[i].size = PARTITION_SIZE;
    }
    std::ofstream file(FILE_NAME, std::ios::out);
    file.close();
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
    if (!blocked.size() && !pending.size() && !ready.size() && !suspended.size()
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
            updatePage(ready.back().getId(), &ready, false, &ready.back());
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
    if ((blocked.size() || suspended.size()) && !ready.size() && current == nullptr)
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
        if (jump >= ERROR || (allBlocked && jump == NEWP && !pending.size()))
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
        allBlocked = false;
        while(pushToMemory());
        if (ready.size() && jump <= INTER) {
            current = new Process(ready.front());
            ready.erase(ready.begin());
            updatePage(current->getId(), &ready, true, current);
        }
        else if (jump <= INTER)
            createDummyProcess(blocked.front().getBlockedTime());
        jump = CONTI;
        current->setQuantum(0);
        controller.readyUp = true;
        controller.memoryUp = true;
        controller.printUpdated();
        allBlocked = false;
        executeProcess(current->getMaxTime() - current->getServiceTime());
        if (jump < INTER) {
            if (jump == QUANTUM && current->getRemTime()){
                ready.push_back(*current);
                updatePage(ready.back().getId(), &ready, false, &ready.back());
            }
            else if (current->getId()) {
                current->setFinishTime(lapsedTime);
                current->setReturnTime(current->getFinishTime() - current->getArrivalTime());
                current->setWaitingTime(current->getReturnTime() - current->getServiceTime());
                if (current->getResponseTime() == NO_RESPONSE_TIME)
                    current->setResponseTime(0);
                if (jump != ERROR)
                    current->calculate();
                updatePage(0, nullptr, false, current, true);
                controller.finishedUp = true;
                finished.push_back(*current);
            }
            delete current;
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
            case 's': case 'S':
                if (suspend())
                    return SUSPENDED;
                return CONTI;
            case 'r': case 'R':
                if (restore())
                    return RESTORED;
                return CONTI;
        }
    }
    return CONTI;
}

unsigned short ProcessManager::inter(long &cont)
{
    if (current->getId()){
        current->setBlockedTime(MAX_BLOCKED_TIME);
        blocked.push_back(*current);
        updatePage(blocked.back().getId(), &blocked, false, &blocked.back());
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
    short id = pending.front().getId();
    short pages = size / PARTITION_SIZE;
    (size % PARTITION_SIZE) ? pages++ : pages;
    if (pages <= emptyFrames && pages && pending.size()) {
        for (short i = 0; i < MEMORY_PARTITIONS; ++i)
            if (!memory[i].id) {
                memory[i] = Page(id, &ready,
                        (size >= PARTITION_SIZE) ? PARTITION_SIZE : size, false);
                size >= PARTITION_SIZE ? size -= PARTITION_SIZE : size = 0;
                emptyFrames--;
                if (!(--pages))
                    break;
            }
        ready.push_back(*pending.begin());
        pending.erase(pending.begin());
        ready.back().setArrivalTime(lapsedTime);
        return true;
    }
    return false;
}

void ProcessManager::updatePage(const short &id, std::vector<Process> *s,
                                bool w, const Process *p, bool freeMemory)
{
    for (short i = 0; i < MEMORY_PARTITIONS; ++i)
        if (p != nullptr) {
            if (memory[i].id == p->getId()){
                memory[i].id = id;
                if (!id)
                    memory[i].size = 0;
                memory[i].state = s;
                memory[i].working = w;
                if (freeMemory)
                    emptyFrames++;
            }
        }
}

bool ProcessManager::restoreToMemory(Process p)
{
    short size = suspended.front().second;
    short pages = size / PARTITION_SIZE;
    (size % PARTITION_SIZE) ? pages++ : pages;
    if (pages <= emptyFrames && pages && suspended.size()){
        for (short i = 0; i < MEMORY_PARTITIONS; ++i)
            if (!memory[i].id) {
                memory[i] = Page(suspended.front().first, &blocked,
                        (size >= PARTITION_SIZE) ? PARTITION_SIZE : size, false);
                size >= PARTITION_SIZE ? size -= PARTITION_SIZE : size = 0;
                emptyFrames--;
                if (!(--pages))
                    break;
            }
        blocked.push_back(p);
        suspended.erase(suspended.begin());
        return true;
    }
    return false;
}

bool ProcessManager::suspend()
{
    if (!blocked.size())
        return false;
    Process tmp = blocked.front();
    std::string str;
    std::string id;
    std::fstream filei(FILE_NAME, std::ios::out | std::ios::in);
    if (filei.is_open()) {
        short pos = filei.tellg();
        while (!filei.eof()){
            pos = filei.tellg();
            getline(filei, id, DEL);
            getline(filei, str);
            if (filei.eof() || id == "0")
                break;
        }
        filei.close();
        std::fstream fileo(FILE_NAME, std::ios::out | std::ios::in);
        fileo.seekp(pos);
        for (short i = 0; i < str.size() + id.size(); ++i)
            fileo << " ";
        fileo.seekp(pos);
        fileo << tmp.getId() << DEL << tmp.getSize()
              << DEL << tmp.getOp().c_str() << DEL << tmp.getMaxTime()
              << DEL << tmp.getRemTime() << DEL << tmp.getServiceTime()
              << DEL << tmp.getArrivalTime() << DEL << tmp.getWaitingTime()
              << DEL << tmp.getResponseTime() << '\n';
        fileo.close();
        suspended.push_back(std::pair<short,short>
                            (tmp.getId(), tmp.getSize()));
        updatePage(0, nullptr, false, &tmp, true);
        blocked.erase(blocked.begin());
        controller.blockedUp = true;
        return true;
    }
    return false;
}

bool ProcessManager::restore()
{
    if (!suspended.size() || suspended.front().second / PARTITION_SIZE > emptyFrames)
        return false;
    Process tmp;
    std::string str;
    std::string id;
    std::fstream filei(FILE_NAME, std::ios::in);
    short pos = filei.tellg();
    while (!filei.eof()) {
        pos = filei.tellg();
        getline(filei, id, DEL);
        if (atoi(id.c_str()) == suspended.front().first) {
            tmp.setId(id);
            getline(filei, str, DEL);
            tmp.setSize(atoi(str.c_str()));
            getline(filei, str, DEL);
            tmp.setOp(str);
            getline(filei, str, DEL);
            tmp.setMaxTime(str);
            getline(filei, str, DEL);
            tmp.setRemTime(atoi(str.c_str()));
            getline(filei, str, DEL);
            tmp.setServiceTime(atoi(str.c_str()));
            getline(filei, str, DEL);
            tmp.setArrivalTime(atoi(str.c_str()));
            getline(filei, str, DEL);
            tmp.setWaitingTime(atoi(str.c_str()));
            getline(filei, str);
            tmp.setResponseTime(atoi(str.c_str()));
            tmp.setBlockedTime(MAX_BLOCKED_TIME);
            tmp.setQuantum(0);
            break;
        }
        getline(filei, str);
        if (filei.eof())
            break;
    }
    filei.close();
    restoreToMemory(tmp);
    std::fstream fileo(FILE_NAME, std::ios::out | std::ios::in);
    fileo.seekp(pos);
    fileo << 0 << DEL;
    fileo.close();
    controller.blockedUp = true;
    return true;
}
