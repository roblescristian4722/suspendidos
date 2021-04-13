#ifndef PROCESS_H
#define PROCESS_H
#include <iostream>
#include <regex>
#include "cursor.h"

class Process
{
private:
    std::string name;
    std::string op;
    std::string result;
    unsigned long id;
    unsigned long maxTime;
    unsigned long remTime;
    unsigned long lapsedTime;
public:
    Process();
    Process(unsigned long id, std::string nombre, std::string operacion,
            unsigned long tiempoMax);
    ~Process();
    
    // Obtiene el nombre del programador del Process
    const std::string& getName() const;
    // Obtiene el tipo de operación a realizar en el Process
    const std::string& getOp() const;
    // Obtiene el ID único del Process
    const unsigned long& getId() const;
    // Obtiene el tiempo máximo estimado de ejecución del Process (en segundos)
    const unsigned long& getMaxTime() const;
    // Obtiene el resultado de la operación 
    const std::string& getResult() const;
    // Obtiene el tiempo restante
    const unsigned long& getRemTime() const;
    // Obtiene el tiepo transcurrido
    const unsigned long& getLapsedTime() const;
    
    // Genera el resultado de la operación
    void calculate();
    // Asigna el resultado de la operación (usado principalmente cuando hay un
    // error en la ejecución)
    void setResult(const std::string& resultado);
    // Asigna el tiempo restante
    void setRemTime(const unsigned long& tiempoRes);
    // Asigna el tiepo transcurrido
    void setLapsedTime(const unsigned long& tiempoTrans);
    // Asigna el nombre del programador del Process
    bool setName(const std::string& nombre);
    // Asigna el tipo de operacion a realizar en el Process
    bool setOp(const std::string& operacion);
    // Asigna el ID único al Process
    bool setId(const std::string& idD);
    // Asigna el tiempo máximo estimado de ejecucón del Process (en segundos)
    bool setMaxTime(const std::string& tiempoMax);
};

#endif // PROCESS_H
