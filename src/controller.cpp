#include "../define/processManager.h"

Controller::Controller(){}
Controller::Controller(ProcessManager *pm)
{
    this->pm = pm;
    readyF.setFrame(1, FRAME_Y, FIELD_WIDTH * 3 + 2,
                    MAX_SIZE_JOBS_FRAME + 2, AMARILLO);
    currentF.setFrame(FIELD_WIDTH * 3 + 4, FRAME_Y, FIELD_WIDTH * 6, 6,
                      VERDE);
    finishedF.setFrame(FIELD_WIDTH * 9 + 5, FRAME_Y, FIELD_WIDTH * 4, 25,
                       CYAN);
    blockedF.setFrame(FIELD_WIDTH * 3 + 15, FRAME_Y + 7,
                      FIELD_WIDTH * 3 + 2, MAX_SIZE_JOBS_FRAME + 3, MORADO);
    memoryF.setFrame(FIELD_WIDTH * 9 + 7 + (FIELD_WIDTH * 4), FRAME_Y,
                      FIELD_WIDTH * 4 + 8, FRAME_Y + MEMORY_PARTITIONS, AZUL);
    nextF.setFrame(1, FRAME_Y + MAX_SIZE_JOBS_FRAME + 3, FIELD_WIDTH * 3 + 2, 6,
                  ROJO);
    suspendedF.setFrame(1, FRAME_Y + MAX_SIZE_JOBS_FRAME + 10, FIELD_WIDTH * 3 + 2, 6);
    stateColors[&pm->finished] = CYAN;
    stateColors[&pm->ready] = VERDE;
    stateColors[&pm->blocked] = ROJO;
    stateColors[&pm->pending] = AMARILLO;
    states[&pm->finished] = "Finalizado";
    states[&pm->ready] = "Listo";
    states[&pm->blocked] = "Bloqueado";
    states[&pm->pending] = "Nuevo";
    readyUp = false;
    finishedUp = false;
    blockedUp = false;
    finishedUp = false;
    memoryUp = false;
    suspendedUp = false;
}

Controller::~Controller(){}

void Controller::redrawBCP()
{
    currentF.drawFrame();
    blockedF.drawFrame();
    finishedF.drawFrame();
    readyF.drawFrame();
    memoryF.drawFrame();
    nextF.drawFrame();
    suspendedF.drawFrame();
    printFrames(false, false, true);
    for (size_t i = 0; i < pm->finished.size(); ++i)
        fillFinished(pm->finished[i]);
    printFrames(true);
    for (size_t i = 0; i < pm->ready.size(); ++i)
        fillReady(pm->ready[i]);
    printUpdated();
}

void Controller::printBCP(const unsigned long lapsedTime, bool fnsh)
{
    std::vector<Process>* queue;
    std::map<std::vector<Process>*, std::string>::iterator it;
    Process tmp;
    // Se itera por cada estado distinto a "en ejecución"
    for (it = states.begin(); it != states.end(); it++) {
        queue = it->first;
        if (fnsh)
            queue = &pm->finished;
        // Se itera por cada proceso
        for (size_t i = 0; i < queue->size(); ++i) {
            std::cout << "ID: " << (*queue)[i].getId()
                      << "\n" << "Estado: " << Cursor::colorText(stateColors[queue],
                                                                 states[queue])
                      << "\n" << "Operacion: " << (*queue)[i].getOp()
                      << "\n" << "Tiempo maximo estimado: " << (*queue)[i].getMaxTime()
                      << "\n" << "Tamaño: " << (*queue)[i].getSize() << " MB"
                      << "\n";
            if (states[queue] != "Nuevo") {
                if (states[queue] != "Finalizado")
                    (*queue)[i].setWaitingTime(lapsedTime - (*queue)[i].getArrivalTime()
                                               - (*queue)[i].getServiceTime());
                std::cout << "Tiempo de llegada: " << (*queue)[i].getArrivalTime()
                          << "\n" << "Tiempo de servicio: " << (*queue)[i].getServiceTime()
                          << "\n" << "Tiempo de espera: " << (*queue)[i].getWaitingTime()
                          << "\n";
                if ((*queue)[i].getResponseTime() != NO_RESPONSE_TIME)
                    std::cout << "Tiempo de respuesta: " << (*queue)[i].getResponseTime()
                              << "\n";
                if (states[queue] == "Finalizado")
                    std::cout << "Resultado: " << (*queue)[i].getResult()
                              << "\n" << "Tiempo de finalizacion: " << (*queue)[i].getFinishTime()
                              << "\n" << "Tiempo de retorno: " << (*queue)[i].getReturnTime()
                              << "\n";
                if (states[queue] == "Bloqueado")
                    std::cout << "Tiempo bloqueado restante: "
                              << (*queue)[i].getBlockedTime() << "\n"
                              << "Tiempo restante: " << (*queue)[i].getRemTime() << "\n";
            }
            std::cout << "\n";
        }
        if (fnsh)
            break;
    }
    if (pm->suspended.size()){
        for (unsigned int i = 0; i < pm->suspended.size(); ++i) {
            tmp = pm->processFromFile(pm->suspended[i].first);
            std::cout << "ID: " << tmp.getId()
                      << "\n" << "Estado: " << Cursor::colorText(AZUL, "Suspendido")
                      << "\n" << "Operación: " << tmp.getOp()
                      << "\n" << "Tiempo máximo estimado: " << tmp.getMaxTime()
                      << "\n" << "Tamaño: " << tmp.getSize()
                      << "\n" << "Tiempo de llegada: " << tmp.getArrivalTime();
            tmp.setWaitingTime(lapsedTime - tmp.getArrivalTime() - tmp.getServiceTime());
            std::cout << "\n" << "Tiempo de espera: " << tmp.getWaitingTime()
                      << "\n" << "Tiempo de servicio: " << tmp.getServiceTime()
                      << "\n" << "Tiempo de respuesta: " << tmp.getResponseTime()
                      << "\n" << "\n";
        } 
    }
    
    // Impresión del proceso en ejecución
    if (pm->current != nullptr)
        if (pm->current->getId()) {
            std::cout << "ID: " << pm->current->getId()
                      << "\n" << "Estado:" << Cursor::colorText(MORADO, "En ejecucion")
                      << "\n" << "Operacion: " << pm->current->getOp()
                      << "\n" << "Tiempo maximo estimado: " << pm->current->getMaxTime()
                      << "\n" << "Tamaño: " << pm->current->getSize() << " MB"
                      << "\n" << "Tiempo restante: " << pm->current->getRemTime()
                      << "\n" << "Tiempo de llegada: " << pm->current->getArrivalTime()
                      << "\n";
            pm->current->setWaitingTime(lapsedTime - pm->current->getArrivalTime()
                                        - pm->current->getServiceTime());
            std::cout << "Tiempo de espera: " << pm->current->getWaitingTime()
                      << "\n" << "Tiempo de servicio: " << pm->current->getServiceTime()
                      << "\n" << "Tiempo de respuesta: " << pm->current->getResponseTime()
                      << "\n" << "\n";
        }
    std::cout.flush();
}

void Controller::fillBlocked()
{
    for (size_t i = 0; i < pm->blocked.size(); ++i) {
        blockedF.print(std::to_string(pm->blocked[i].getId()), BLANCO, false,
                FIELD_WIDTH + 3);
        blockedF.print(std::to_string(pm->blocked[i].getBlockedTime()), BLANCO, true,
                FIELD_WIDTH);
    }
}

void Controller::fillCurrent()
{
    currentF.print(std::to_string(pm->current->getId()), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(pm->current->getQuantum()), BLANCO, false, FIELD_WIDTH);
    currentF.print(pm->current->getOp(), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(pm->current->getMaxTime()), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(pm->current->getRemTime()), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(pm->current->getServiceTime()), BLANCO, true, FIELD_WIDTH);
}

void Controller::fillFinished(Process &p)
{
    finishedF.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    finishedF.print(p.getOp(), BLANCO, false, FIELD_WIDTH);
    finishedF.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    finishedF.print(p.getResult(), BLANCO, true, FIELD_WIDTH);
}

void Controller::fillReady(Process &p)
{
    readyF.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    readyF.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    readyF.print(std::to_string(p.getServiceTime()), BLANCO, true, FIELD_WIDTH);
}

void Controller::fillNext(Process &p)
{
    nextF.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    nextF.print(std::to_string(p.getSize()) + " MB", BLANCO, true, FIELD_WIDTH);
}

void Controller::fillSuspended(std::pair<short, short> p)
{
    suspendedF.print(std::to_string(p.first), BLANCO, false, FIELD_WIDTH);
    suspendedF.print(std::to_string(p.second) + " MB", BLANCO, true, FIELD_WIDTH);
}

void Controller::fillMemory(const short &f, const Page &p)
{
    memoryF.print(std::to_string(f), BLANCO, false, FIELD_WIDTH);
    memoryF.print(std::to_string(p.size) + "/" + std::to_string(PARTITION_SIZE) + " MB",
                  BLANCO, false, FIELD_WIDTH);
    if (p.id == -1) {
        memoryF.print("SO", BLANCO, false, FIELD_WIDTH);
        memoryF.print("sistema", BLANCO, true);

    }
    else if (p.id){
        memoryF.print(std::to_string(p.id), BLANCO, false, FIELD_WIDTH);
        if (!p.working)
            memoryF.print(states[p.state], BLANCO, true);
        else
            memoryF.print("En ejecucion", BLANCO, true);
    }
    else {
        memoryF.print("libre", BLANCO, false, FIELD_WIDTH);
        memoryF.print("-", BLANCO, true);
    }
}

void Controller::printFrames(bool rdy, bool act, bool fnshd, bool bloq, bool memory,
                             bool next, bool suspended)
{
    if (rdy) {
        readyF.update("Procesos listos:", BLANCO, true);
        readyF.print("ID      TME     TMT", BLANCO, true);
    }
    if (act) {
        currentF.update("Proceso en ejecucion:", BLANCO, true);
        currentF.print("ID      QTM     OP      TME     TMR     TMT", BLANCO, true);
    }
    if (fnshd){
        finishedF.print("Procesos terminados:", BLANCO, true);
        finishedF.print("ID      OP      TME     RES", BLANCO, true);
    }
    if (bloq) {
        blockedF.update("Procesos bloqueados:", BLANCO, true);
        blockedF.print("ID         TRB", BLANCO, true);
    }
    if (memory) {
        memoryF.update("Tabla de paginas:", BLANCO, true);
        memoryF.print("MARCO   TAM     ID      ESTADO", BLANCO, true);
    }
    if (next){
        nextF.update("Proceso siguiente:", BLANCO, true);
        nextF.print("ID      TAM", BLANCO, true);
    }
    if (suspended){
        suspendedF.update("Proceso suspendido:", BLANCO, true);
        suspendedF.print("ID      TAM", BLANCO, true);
    }
}

void Controller::printCounters(const size_t &pending, const size_t &lapsedTime,
                               const unsigned int quantum)
{
    std::string auxStr;
    auxStr = "Procesos nuevos: " + std::to_string(pending)
        + "\nTiempo transcurrido: " + std::to_string(lapsedTime)
        + "\nValor del Quantum: " + std::to_string(quantum);
        Cursor::rmPrint(1, 2, auxStr);
}

void Controller::initFrames()
{
    Cursor::hideCursor();
    Cursor::clrscr();
    readyF.drawFrame();
    blockedF.drawFrame();
    finishedF.drawFrame();
    currentF.drawFrame();
    memoryF.drawFrame();
    nextF.drawFrame();
    suspendedF.drawFrame();
    printFrames(true, true, true, true, true, true, true);
}

void Controller::endFrames()
{
    readyF.rmContent();
    currentF.rmContent();
    printFrames(true, true);
    Cursor::showCursor();
}

void Controller::printUpdated()
{
    printCounters(pm->pending.size(), pm->lapsedTime, pm->quantum);
    if (readyUp) {
        printFrames(true);
        for (size_t i = 0; i < pm->ready.size(); ++i)
            fillReady(pm->ready[i]);
        readyUp = false;
    }
    if (blockedUp) {
        printFrames(false, false, false, true);
        fillBlocked();
        currentF.rmContent();
    }
    if (finishedUp) {
        fillFinished(pm->finished.back());
        finishedUp = false;
    }
    if (memoryUp){
        printFrames(false, false, false, false, true);
        for (short i = 0; i < MEMORY_PARTITIONS; ++i)
            fillMemory(i, pm->memory[i]);
    }
    printFrames(false, false, false, false, false, true);
    if (pm->pending.size())
        fillNext(pm->pending.front());
    printFrames(false, false, false, false, false, false, true);
    if (pm->suspended.size())
        fillSuspended(pm->suspended.front());
    if (pm->current != nullptr) {
        printFrames(false, true);
        fillCurrent();
    }
    std::cout.flush();
}
