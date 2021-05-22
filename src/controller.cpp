#include "../define/processManager.h"

Controller::Controller(){}
Controller::Controller(std::vector<Process> *pending, std::vector<Process> *ready,
                       std::vector<Process> *finished, std::vector<Process> *blocked,
                       Process *current,
                       std::map<std::vector<Process> *, std::string> *states,
                       std::map<std::vector<Process> *, char> *stateColors,
                       unsigned long *lapsedTime, unsigned int *quantum)
{
    this->pending = pending;
    this->ready = ready;
    this->finished = finished;
    this->blocked = blocked;
    this->states = states;
    this->current = current;
    this->stateColors = stateColors;
    readyF.setFrame(1, FRAME_Y, FIELD_WIDTH * 3 + 2,
                    MAX_SIZE_JOBS_FRAME + 2, AMARILLO);
    currentF.setFrame(FIELD_WIDTH * 3 + 4, FRAME_Y, FIELD_WIDTH * 6, 6,
                      VERDE);
    finishedF.setFrame(FIELD_WIDTH * 9 + 5, FRAME_Y, FIELD_WIDTH * 4, 25,
                       CYAN);
    blockedF.setFrame(1, FRAME_Y + MAX_SIZE_JOBS_FRAME + 2,
                      FIELD_WIDTH * 2 + 2, MAX_SIZE_JOBS_FRAME + 3, MORADO);
    (*stateColors)[&(*finished)] = CYAN;
    (*stateColors)[&(*ready)] = VERDE;
    (*stateColors)[&(*blocked)] = ROJO;
    (*stateColors)[&(*pending)] = AMARILLO;
    (*states)[&(*finished)] = "Finalizado";
    (*states)[&(*ready)] = "Listo";
    (*states)[&(*blocked)] = "Bloqueado";
    (*states)[&(*pending)] = "Nuevo";
    this->lapsedTime = lapsedTime;
    this->quantum = quantum;
    readyUp = false;
    finishedUp = false;
    blockedUp = false;
    finishedUp = false;
}

Controller::~Controller(){}

void Controller::redrawBCP()
{
    currentF.drawFrame();
    blockedF.drawFrame();
    finishedF.drawFrame();
    readyF.drawFrame();
    printFrames(false, false, true);
    for (size_t i = 0; i < (*finished).size(); ++i)
        fillFinished((*finished)[i]);
    readyF.drawFrame();
    printFrames(true);
    for (size_t i = 0; i < (*ready).size(); ++i)
        fillReady((*ready)[i]);
    printUpdated();
}

void Controller::printBCP(const unsigned long lapsedTime, bool fnsh)
{
    std::vector<Process>* queue;
    std::map<std::vector<Process>*, std::string>::iterator it;
    // Se itera por cada estado distinto a "en ejecución"
    for (it = (*states).begin(); it != (*states).end(); it++) {
        queue = it->first;
        if (fnsh)
            queue = &(*finished);
        // Se itera por cada proceso
        for (size_t i = 0; i < queue->size(); ++i) {
            std::cout << "ID: " << (*queue)[i].getId() << std::endl
                      << "Estado: " << Cursor::colorText((*stateColors)[queue],
                                                    (*states)[queue]) << std::endl;
            if ((*states)[queue] != "Nuevo") {
                if ((*states)[queue] != "Finalizado")
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
                if ((*states)[queue] == "Finalizado")
                    std::cout << "Resultado: " << (*queue)[i].getResult() << std::endl
                            << "Tiempo de finalizacion: " << (*queue)[i].getFinishTime()
                            << std::endl
                            << "Tiempo de retorno: " << (*queue)[i].getReturnTime()
                            << std::endl;
                if ((*states)[queue] == "Bloqueado")
                    std::cout << "Tiempo bloqueado restante: "
                              << (*queue)[i].getBlockedTime() << std::endl;
            }
            std::cout << std::endl;
        }
        if (fnsh)
            break;
    }
    // Impresión del proceso en ejecución
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
    std::cout.flush();
}

void Controller::fillBlocked()
{
    for (size_t i = 0; i < (*blocked).size(); ++i) {
        blockedF.print(std::to_string((*blocked)[i].getId()), BLANCO, false,
                FIELD_WIDTH);
        blockedF.print(std::to_string((*blocked)[i].getBlockedTime()), BLANCO, true,
                FIELD_WIDTH);
    }
}

void Controller::fillCurrent()
{
    currentF.print(std::to_string(current->getId()), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(current->getQuantum()), BLANCO, false, FIELD_WIDTH);
    currentF.print(current->getOp(), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(current->getMaxTime()), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(current->getRemTime()), BLANCO, false, FIELD_WIDTH);
    currentF.print(std::to_string(current->getServiceTime()), BLANCO, true, FIELD_WIDTH);
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

void Controller::printFrames(bool rdy, bool act, bool fnshd, bool bloq)
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
        blockedF.print("ID      TRB", BLANCO, true);
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
    printFrames(true, true, true, true);
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
    printCounters(pending->size(), *lapsedTime, *quantum);
    if (readyUp) {
        printFrames(true);
        for (size_t i = 0; i < ready->size(); ++i)
            fillReady((*ready)[i]);
        readyUp = false;
    }
    if (blockedUp) {
        printFrames(false, false, false, true);
        fillBlocked();
        currentF.rmContent();
    }
    if (finishedUp) {
        fillFinished(finished->back());
        finishedUp = false;
    }
    if (current != nullptr) {
        printFrames(false, true);
        fillCurrent();
    }
    std::cout.flush();
}