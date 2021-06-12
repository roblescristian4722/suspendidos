#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include <regex>
#include <fstream>
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #define GETCH(X) X = getch()
#else
    #include "kbhitLinux.h"
    #define GETCH(X) set_echo();\
                     X = getchar();\
                     unset_echo()
#endif
#include "process.h"
#include "cursor.h"
#define FIELD_WIDTH 8
#define MAX_READY_JOB_AMOUNT 4
#define FRAME_Y 5
#define MAX_SIZE_JOBS_FRAME 24
#define MAX_BLOCKED_TIME 5
#define NO_RESPONSE_TIME -1
#define PARTITION_SIZE 4
#define MAX_PAGE_SIZE 25
#define MIN_PAGE_SIZE 5
#define MEMORY_SIZE 180
static const short MEMORY_PARTITIONS = MEMORY_SIZE / PARTITION_SIZE;

class Controller;
class ProcessManager;

enum ExecResult{
    CONTI = 0,
    INTER,
    ERROR,
    BCP,
    NEWP,
    QUANTUM,
};

struct Page
{ 
    short id = 0;
    std::vector<Process> *state = nullptr;
    short size = 0;
    Page(){}
    Page(const short &id, std::vector<Process> *state, const short &size);
};

// Clase que funciona como controlador de arquitectura MVC para transferencia
// de información. Contiene métodos para la impresión de ventanas y datos.
class Controller
{
private:
    friend class ProcessManager;
    
    bool readyUp;
    bool finishedUp;
    bool blockedUp;
    bool memoryUp;
    unsigned long *lapsedTime;
    unsigned int *quantum;
    Frame readyF;
    Frame finishedF;
    Frame currentF;
    Frame blockedF;
    Frame memoryF;
    std::vector<Process> *pending;
    std::vector<Process> *ready;
    Process *current;
    std::vector<Process> *finished;
    std::vector<Process> *blocked;
    std::map<std::vector<Process> *, std::string> states;
    std::map<std::vector<Process> *, char> stateColors;
    Page (*memory)[MEMORY_PARTITIONS];
    std::vector<std::pair<short, short>> availableProcesses;

    // Se proveen datos para el frame de procesos bloqueados
    void fillBlocked();
    // Se proveen datos para el frame del proceso actual
    void fillCurrent();
    // Se proveen datos para el frame de procesos terminados
    void fillFinished(Process &p);
    // Se proveen datos para el frame de procesos listos
    void fillReady(Process &p);
    // Se proveen datos para el frame de procesos listos
    void fillMemory(const short &f, const Page &p);
    // Imprime en pantalla el BCP de los procesos registrados
    void printBCP(const unsigned long lapsedTime, bool finished = false);
    // Inicializa las 3 ventanas para la ejecición de procesos
    void printFrames(bool pend = false, bool act = false,
                     bool term = false, bool bloq = false, bool memory = false);
    // Vuelve a dibujar los Frames ready y blocked después de mostrar el BCP
    void redrawBCP();
    void printCounters(const size_t &pending, const size_t &lapsedTime,
                       const unsigned int quantum);
    void initFrames();
    void endFrames();
    void printUpdated();

public:
    Controller();
    Controller(std::vector<Process> *pending, std::vector<Process> *ready,
               std::vector<Process> *finished, std::vector<Process> *blocked,
               Process *current, unsigned long *lapsedTime, unsigned int *quantum,
               Page (*memory)[MEMORY_PARTITIONS]);
    ~Controller();
};

// Clase para la gestión de procesos
class ProcessManager
{
private:
    friend class Controller;

    std::vector<Process> pending;
    std::vector<Process> ready;
    Process* current;
    std::vector<Process> finished;
    std::vector<Process> blocked;
    static std::map<std::string, bool> idsUsed;
    unsigned long lastId;
    unsigned long lapsedTime;
    unsigned int quantum;
    unsigned short jump;
    bool allBlocked;
    Page memory[MEMORY_PARTITIONS];
    short emptyFrames;
    Controller controller;

    // Genera tiempo aleatorio entre un rango de 6 a 15 segundos
    unsigned long generateTime();
    // Genera una opreración aleatoria
    std::string generateOp();
    // Genera el peso de un proceso de forma aleatoria
    short generateSize();
    // Función genérica que llama a los setters de un Process, se ejecuta de
    // en un bucle hasta que el setter retorne true
    void obtainProcess(const unsigned long& cont);
    // Comienza con la ejecución de los procesos
    void execute();
    // Ejecuta el proceso actual
    void executeProcess(long execTime);
    // Interrumpe un Process y lo pone al final de la cola de procesos del
    // ProcessManager actual
    unsigned short inter(long &cont);
    // Pausa un Process hasta que usuario presione "c" para continuar
    void pause(const bool &bcp = false);
    // Escucha el teclado y ejecuta uno de los posibles casos con
    // kbhit: i, p, c, e, b
    unsigned short keyListener(long& cont);
    // Retorna true si es necesario crear un proceso vacío y retorna false en
    // caso contario
    bool dummyProcess();
    void createDummyProcess(const long& execTime);
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
    bool pushToMemory();

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
