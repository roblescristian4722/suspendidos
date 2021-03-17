#ifndef LOTE_H
#define LOTE_H
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include <regex>
#include "proceso.h"
#include "cursor.h"
#define FIELD_WIDTH 8
#define BATCH_MAX_CAPACITY 5

class Lote
{
private:
    unsigned long id;
    std::vector<Proceso> procPend;
    Proceso* procActual;
    std::vector<Proceso> procTerm;
    static std::map<std::string, bool> idsUsed;
    static unsigned long tiempoTotal;

    // Establece el ID del proceso, retorna true si el proceso fue exitoso y 
    // retorna false en caso de que el ID sea incorrecto o ya esté en uso
    void capturarCampo(std::string msj, std::string msjError,
                       Proceso& proc,
                       bool(Proceso::*metodo)(const std::string&));
    // Sobrecarga de capturar campo que tiene un parámetro extra en el que se
    //
    // brinda un map para verificar que el ID capturado sea único
    void capturarCampo(std::string msj, std::string msjError,
                       Proceso& proc,
                       bool(Proceso::*metodo)(const std::string&,
                                              std::map<std::string,bool>*),
                       std::map<std::string, bool>* idsUsed);
    // Inicializa las 3 ventanas para la ejecición de procesos
    void imprimirVentanas(Frame* pend = nullptr, Frame* act = nullptr,
                          Frame* term = nullptr);
    // Genera tiempo aleatorio entre un rango de 6 a 15 segundos
    unsigned long generarTiempoProceso();
    // Genera una opreración aleatoria
    std::string generarOperacionProceso();
    // Función genérica que llama a los setters de un proceso, se ejecuta de
    // en un bucle hasta que el setter retorne true
    void capturarProceso(const unsigned long& cont);
    // Se itera por la lista de procesos pendientes y se ejecutan de en orden
    // de uno en uno
    void ejecutarProcesos();
    // Imprime los datos del proceso en un marco
    void llenarMarco(Frame& marco, Proceso& proc, bool actual, bool term);

public:
    Lote();
    // Constructor copy
    Lote(const Lote& lote);
    ~Lote();
    const Lote& operator =(const Lote& lote);

    // Imprime en pantalla los procesos pendientes de ejecución en orden
    std::vector<Proceso>& getProcesosPendientes() const;
    // Imprime en pantalla los procesos pendientes de ejecución en orden
    void printProcesosTerminados();
    // Retorna un puntero hacia el proceso en ejecución
    Proceso* getProcesoActual() const;
    // Retorna el ID del lote
    const unsigned long& getId() const;
    // Establece el ID del lote
    void setId(const unsigned long& id); 
    // Función que comienza la obtención de procesos
    void iniciar();
};

#endif // LOTE_H
