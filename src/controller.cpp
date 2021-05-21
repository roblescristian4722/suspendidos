#include "../define/processManager.h"

Controller::Controller(){}
Controller::~Controller(){}

void Controller::printBCP(ProcessManager *pm, bool fnsh)
{
    std::vector<Process>* queue;
    std::map<std::vector<Process>*, std::string>::iterator it;
    // Se itera por cada estado distinto a "en ejecución"
    for (it = pm->states.begin(); it != pm->states.end(); it++) {
        queue = it->first;
        if (fnsh)
            queue = &pm->finished;
        // Se itera por cada proceso
        for (size_t i = 0; i < queue->size(); ++i) {
            std::cout << "ID: " << (*queue)[i].getId() << std::endl
                      << "Estado: " << Cursor::colorText(pm->stateColors[queue],
                                                    pm->states[queue]) << std::endl;
            if (pm->states[queue] != "Nuevo") {
                if (pm->states[queue] != "Finalizado")
                    (*queue)[i].setWaitingTime(pm->lapsedTime - (*queue)[i].getArrivalTime()
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
                if (pm->states[queue] == "Finalizado")
                    std::cout << "Resultado: " << (*queue)[i].getResult() << std::endl
                            << "Tiempo de finalizacion: " << (*queue)[i].getFinishTime()
                            << std::endl
                            << "Tiempo de retorno: " << (*queue)[i].getReturnTime()
                            << std::endl;
                if (pm->states[queue] == "Bloqueado")
                    std::cout << "Tiempo bloqueado restante: "
                              << (*queue)[i].getBlockedTime() << std::endl;
            }
            std::cout << std::endl;
        }
        if (fnsh)
            break;
    }
    // Impresión del proceso en ejecución
    if (pm->current->getId()) {
        std::cout << "ID: " << pm->current->getId() << std::endl
                << "Estado:" << Cursor::colorText(MORADO, "En ejecucion")
                << std::endl
                << "Operacion: " << pm->current->getOp() << std::endl
                << "Tiempo maximo estimado: " << pm->current->getMaxTime()
                << std::endl
                << "Tiempo restante: " << pm->current->getRemTime()
                << std::endl
                << "Tiempo de llegada: " << pm->current->getArrivalTime()
                << std::endl;
        pm->current->setWaitingTime(pm->lapsedTime - pm->current->getArrivalTime()
                                - pm->current->getServiceTime());
        std::cout << "Tiempo de espera: " << pm->current->getWaitingTime()
                << std::endl
                << "Tiempo de servicio: " << pm->current->getServiceTime()
                << std::endl
                << "Tiempo de respuesta: " << pm->current->getResponseTime()
                << std::endl << std::endl;
    }
}

void Controller::initFrames(ProcessManager* pm)
{
    pm->readyF.setFrame(1, FRAME_Y, FIELD_WIDTH * 3 + 2,
                          MAX_SIZE_JOBS_FRAME + 2, AMARILLO);
    pm->currentF.setFrame(FIELD_WIDTH * 3 + 4, FRAME_Y, FIELD_WIDTH * 6, 6,
                          VERDE);
    pm->finishedF.setFrame(FIELD_WIDTH * 9 + 5, FRAME_Y, FIELD_WIDTH * 4, 25,
                           CYAN);
    pm->blockedF.setFrame(1, FRAME_Y + MAX_SIZE_JOBS_FRAME + 2,
                          FIELD_WIDTH * 2 + 2, MAX_SIZE_JOBS_FRAME + 3, MORADO);
}

void Controller::initStateColors(ProcessManager *pm)
{
    pm->stateColors[&pm->finished] = CYAN;
    pm->stateColors[&pm->ready] = VERDE;
    pm->stateColors[&pm->blocked] = ROJO;
    pm->stateColors[&pm->pending] = AMARILLO;
}

void Controller::initStates(ProcessManager *pm)
{
    pm->states[&pm->finished] = "Finalizado";
    pm->states[&pm->ready] = "Listo";
    pm->states[&pm->blocked] = "Bloqueado";
    pm->states[&pm->pending] = "Nuevo";
}

void Controller::fillBlocked(ProcessManager *pm)
{
    for (size_t i = 0; i < pm->blocked.size(); ++i) {
        pm->blockedF.print(std::to_string(pm->blocked[i].getId()), BLANCO, false,
                FIELD_WIDTH);
        pm->blockedF.print(std::to_string(pm->blocked[i].getBlockedTime()), BLANCO, true,
                FIELD_WIDTH);
    }
}

void Controller::reDrawFinished(ProcessManager *pm)
{
    pm->finishedF.drawFrame();
    printFrames(pm, false, false, true);
    for (size_t i = 0; i < pm->finished.size(); ++i)
        fillFinished(pm, pm->finished[i]);
}

void Controller::reDrawReady(ProcessManager *pm)
{
    pm->readyF.drawFrame();
    printFrames(pm, true);
    for (size_t i = 0; i < pm->ready.size(); ++i)
        fillReady(pm, pm->ready[i]);
}

void Controller::fillCurrent(ProcessManager *pm)
{
    pm->currentF.print(std::to_string(pm->current->getId()), BLANCO, false, FIELD_WIDTH);
    pm->currentF.print(std::to_string(pm->current->getQuantum()), BLANCO, false, FIELD_WIDTH);
    pm->currentF.print(pm->current->getOp(), BLANCO, false, FIELD_WIDTH);
    pm->currentF.print(std::to_string(pm->current->getMaxTime()), BLANCO, false, FIELD_WIDTH);
    pm->currentF.print(std::to_string(pm->current->getRemTime()), BLANCO, false, FIELD_WIDTH);
    pm->currentF.print(std::to_string(pm->current->getServiceTime()), BLANCO, true, FIELD_WIDTH);
}

void Controller::fillFinished(ProcessManager *pm, Process &p)
{
    pm->finishedF.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    pm->finishedF.print(p.getOp(), BLANCO, false, FIELD_WIDTH);
    pm->finishedF.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    pm->finishedF.print(p.getResult(), BLANCO, true, FIELD_WIDTH);
}

void Controller::fillReady(ProcessManager *pm, Process &p)
{
    pm->readyF.print(std::to_string(p.getId()), BLANCO, false, FIELD_WIDTH);
    pm->readyF.print(std::to_string(p.getMaxTime()), BLANCO, false, FIELD_WIDTH);
    pm->readyF.print(std::to_string(p.getServiceTime()), BLANCO, true, FIELD_WIDTH);
}

void Controller::printFrames(ProcessManager *pm, bool rdy, bool act, bool fnshd, bool bloq)
{
    if (rdy) {
        pm->readyF.print("Procesos listos:", BLANCO, true);
        pm->readyF.print("ID      TME     TMT", BLANCO, true);
    }
    if (act) {
        pm->currentF.update("Proceso en ejecucion:", BLANCO, true);
        pm->currentF.print("ID      QTM     OP      TME     TMR     TMT", BLANCO, true);
    }
    if (fnshd){
        pm->finishedF.print("Procesos terminados:", BLANCO, true);
        pm->finishedF.print("ID      OP      TME     RES", BLANCO, true);
    }
    if (bloq) {
        pm->blockedF.update("Procesos bloqueados:", BLANCO, true);
        pm->blockedF.print("ID      TRB", BLANCO, true);
    }
}
