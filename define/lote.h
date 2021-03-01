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

    void capturarCampo(std::string msj, std::string msjError,
                       Proceso& proc,
                       bool(Proceso::*metodo)(const std::string&));
    void capturarCampo(std::string msj, std::string msjError,
                       Proceso& proc,
                       bool(Proceso::*metodo)(const std::string&,
                                              std::map<std::string,bool>*),
                       std::map<std::string, bool>* IDs);

public:
    Lote();
    Lote(const Lote& lote);
    ~Lote();
    const Lote& operator =(const Lote& lote);
    
    void getProcesosPendientes();
    void getProcesosTerminados();
    Proceso* getProcesoActual() const;
    const unsigned long& getID() const;
    bool setID(const std::string& ID, std::map<std::string,bool>* IDs);
    
    void capturarLote();
    void ejecutarProcesos();
};

#endif // LOTE_H
