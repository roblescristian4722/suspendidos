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

class Lote
{
private:
    unsigned long ID;
    std::vector<Proceso> procPend;
    Proceso* procActual;
    std::vector<Proceso> procTerm;
    std::map<std::string, bool> IDsUsados;
    unsigned long tiempoTotal = 0;

    // Establece el ID del proceso, retorna true si el proceso fue exitoso y 
    // retorna false en caso de que el ID sea incorrecto o ya esté en uso
    void capturarCampo(std::string msj, std::string msjError,
                       Proceso& proc,
                       bool(Proceso::*metodo)(const std::string&));
    // Sobrecarga de capturar campo que tiene un parámetro extra en el que se
    // brinda un map para verificar que el ID capturado sea único
    void capturarCampo(std::string msj, std::string msjError,
                       Proceso& proc,
                       bool(Proceso::*metodo)(const std::string&,
                                              std::map<std::string,bool>*),
                       std::map<std::string, bool>* IDs);

public:
    Lote();
    // Constructor copy
    Lote(const Lote& lote);
    ~Lote();
    const Lote& operator =(const Lote& lote);

    // Imprime los datos del proceso en un marco
    void llenarMarco(Frame& marco, Proceso& proc, bool actual, bool term);
    // Imprime en pantalla los procesos pendientes de ejecución en orden
    void getProcesosPendientes();
    // Imprime en pantalla los procesos pendientes de ejecución en orden
    void getProcesosTerminados();
    // Retorna un puntero hacia el proceso en ejecución
    Proceso* getProcesoActual() const;
    // Retorna el ID del lote
    const unsigned long& getID() const;
    // Establece el ID del lote
    bool setID(const std::string& ID, std::map<std::string,bool>* IDs); 
    // Función genérica que llama a los setters de un proceso, se ejecuta de
    // en un bucle hasta que el setter retorne true
    void capturarLote();
    // Se itera por la lista de procesos pendientes y se ejecutan de en orden
    // de uno en uno
    void ejecutarProcesos();
    // Función que comienza la captura de procesos
    void iniciarCaptura();
};

#endif // LOTE_H
