#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include <regex>
#include <conio.h>
#include "process.h"
#include "cursor.h"
#define FIELD_WIDTH 8
#define MAX_READY_JOB_AMOUNT 4
#define FRAME_Y 5
#define MAX_SIZE_JOBS_FRAME 8
#define MAX_BLOCKED_TIME 5
#define NO_RESPONSE_TIME -1

enum ExecResult{
    CONTI = 0,
    INTER,
    ERROR,
    BCP,
    NEWP
};

class ProcessManager
{
private:
    unsigned long id;
    std::vector<Process> pending;
    std::vector<Process> ready;
    Process* current;
    std::vector<Process> finished;
    std::vector<Process> blocked;
    Frame readyF;
    Frame finishedF;
    Frame currentF;
    Frame blockedF;
    std::map<std::vector<Process>*, std::string> states;
    std::map<std::vector<Process>*, char> stateColors;
    static std::map<std::string, bool> idsUsed;
    unsigned long lastId;
    unsigned long lapsedTime;

    // Establece el ID del Process, retorna true si el Process fue exitoso y 
    // retorna false en caso de que el ID sea incorrecto o ya esté en uso
    void obtainField(std::string msj, std::string msjError,
                       Process& proc,
                       bool(Process::*metodo)(const std::string&));
    // Sobrecarga de capturar campo que tiene un parámetro extra en el que se
    //
    // brinda un map para verificar que el ID capturado sea único
    void obtainField(std::string msj, std::string msjError,
                       Process& proc,
                       bool(Process::*metodo)(const std::string&,
                                              std::map<std::string,bool>*),
                       std::map<std::string, bool>* idsUsed);
    // Inicializa las 3 ventanas para la ejecición de procesos
    void printFrames(bool pend = false, bool act = false, bool term = false,
                     bool bloq = false);
    // Genera tiempo aleatorio entre un rango de 6 a 15 segundos
    unsigned long generateTime();
    // Genera una opreración aleatoria
    std::string generateOp();
    // Función genérica que llama a los setters de un Process, se ejecuta de
    // en un bucle hasta que el setter retorne true
    void obtainProcess(const unsigned long& cont);
    // Se itera por la lista de procesos pendientes y se ejecutan de en orden
    // de uno en uno
    void executeProcess();
    // Interrumpe un Process y lo pone al final de la cola de procesos del
    // ProcessManager actual
    unsigned short inter(long &cont);
    // Pausa un Process hasta que usuario presione "c" para continuar
    void pause(const bool &bcp = false);
    // Escucha el teclado y ejecuta uno de los posibles casos con
    // kbhit: i, p, c, e, b
    unsigned short keyListener(long& cont);
    // Se proveen datos para el frame de procesos bloqueados
    void fillBlocked();
    // Se proveen datos para el frame del proceso actual
    void fillCurrent();
    // Se proveen datos para el frame de procesos terminados
    void fillFinished(Process &p);
    // Se proveen datos para el frame de procesos listos
    void fillReady(Process &p);
    // Retorna true si es necesario crear un proceso vacío y retorna false en
    // caso contario
    bool dummyProcess();
    // Carga de uno a uno los procesos nuevos en memoria (máximo 5 al mismo
    // tiempo) hasta que la cola de procesos nuevos esté vacía
    void pendingToReady();
    // Calcula los procesos en memoria
    size_t onMemory();
    // Ejecuta procesos hasta que todas las colas estén vacías
    bool processLeft();
    // Itera por la cola de bloqueados reduciendo el contador de bloqueo de cada
    // uno, si el contador de un proceso llega a cero se inserta en la cola de
    // listos
    void checkBlocked();
    // Imprime en pantalla el BCP de los procesos registrados
    void printBCP(bool finished = false);
    // Vuelve a imprimir la ventana de procesos listos
    void reDrawReady();
    // Vuelve a imprimir la ventana de procesos terminados
    void reDrawFinished();

public :
    ProcessManager();
    ~ProcessManager();

    // Imprime en pantalla los procesos listos de ejecución en orden
    const std::vector<Process>& getReady() const;
    // Imprime en pantalla los procesos finalizados de ejecución en orden
    void printFinished();
    // Retorna un puntero hacia el Process en ejecución
    Process* getCurrent() const;
    // Retorna el ID del ProcessManager
    const unsigned long& getId() const;
    // Establece el ID del ProcessManager
    void setId(const unsigned long& id); 
    // Función que comienza la obtención de procesos
    void init();
};

#endif // PROCESS_MANAGER_H
