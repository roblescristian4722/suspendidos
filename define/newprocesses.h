#ifndef NEW_PROCESSES_H
#define NEW_PROCESSES_H
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
#define MAX_JOB_AMOUNT 4
#define FRAME_Y 4
#define MAX_SIZE_JOBS_FRAME 8

class NewProcesses
{
private:
    unsigned long id;
    std::vector<Process> ready;
    Process* current;
    std::vector<Process> finished;
    static std::map<std::string, bool> idsUsed;
    static unsigned long lapsedTime;

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
    void printFrames(Frame* pend = nullptr, Frame* act = nullptr,
                          Frame* term = nullptr, Frame* bloq = nullptr);
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
    // Imprime los datos del Process en un marco
    void fillFrame(Frame& marco, Process& proc, bool actual, bool term);
    // Interrumpe un Process y lo pone al final de la cola de procesos del
    // NewProcesses actual
    bool inter(long &cont);
    // Pausa un Process hasta que usuario presione "c" para continuar
    void pause();
    // Escucha el teclado y ejecuta uno de los posibles casos con
    // kbhit: i, p, c, e
    bool keyListener(long& cont);

public:
    NewProcesses();
    // Constructor copy
    NewProcesses(const NewProcesses& NewProcesses);
    ~NewProcesses();
    const NewProcesses& operator =(const NewProcesses& NewProcesses);

    // Imprime en pantalla los procesos listos de ejecución en orden
    const std::vector<Process>& getReady() const;
    // Imprime en pantalla los procesos finalizados de ejecución en orden
    void printFinished();
    // Retorna un puntero hacia el Process en ejecución
    Process* getCurrent() const;
    // Retorna el ID del NewProcesses
    const unsigned long& getId() const;
    // Establece el ID del NewProcesses
    void setId(const unsigned long& id); 
    // Función que comienza la obtención de procesos
    void init();
};

#endif // NEW_PROCESSES_H
