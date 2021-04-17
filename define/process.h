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
    unsigned short maxTime;
    unsigned short remTime;
    unsigned short serviceTime;
    unsigned short arrivalTime;
    unsigned short finishTime;
    unsigned short returnTime;
    unsigned short waitingTime;
    unsigned short blockedTime;
    short responseTime;

public:
    Process();
    Process(unsigned long id, std::string nombre, std::string operacion,
            unsigned short tiempoMax);
    ~Process();
    
    // Obtiene el nombre del programador del Process
    const std::string& getName() const;
    // Obtiene el tipo de operación a realizar en el Process
    const std::string& getOp() const;
    // Obtiene el ID único del Process
    const unsigned long& getId() const;
    // Obtiene el tiempo máximo estimado de ejecución del Process (en segundos)
    const unsigned short& getMaxTime() const;
    // Obtiene el resultado de la operación 
    const std::string& getResult() const;
    // Obtiene el tiempo restante
    const unsigned short& getRemTime() const;
    // Obtiene el tiepo transcurrido o de servicio
    const unsigned short& getServiceTime() const;
    // Obtiene el tiempo de llegada
    const unsigned short &getArrivalTime() const;
    // Obtiene el tiempo de finalización
    const unsigned short& getFinishTime() const;
    // Obtiene el tiempo de retorno
    const unsigned short& getReturnTime() const;
    // Obtiene el tiempo de espera
    const unsigned short& getWaitingTime() const;
    // Obtiene el tiempo de bloqueo
    const unsigned short &getBlockedTime() const;
    // Obtiene el tiempo de respuesta
    const short &getResponseTime() const;

    // Genera el resultado de la operación
    void calculate();
    // Asigna el resultado de la operación (usado principalmente cuando hay un
    // error en la ejecución)
    void setResult(const std::string& resultado);
    // Asigna el tiempo restante
    void setRemTime(const unsigned short& tiempoRes);
    // Asigna el tiepo transcurrido
    void setServiceTime(const unsigned short &serviceTime);
    // Asigna el tiempo de llegada
    void setArrivalTime(const unsigned short &arrivalTime);
    // Asigna el tiempo de finalización
    void setFinishTime(const unsigned short &finishTime);
    // Asigna el tiempo de retorno
    void setReturnTime(const unsigned short &returnTime);
    // Asigna el tiempo de espera
    void setWaitingTime(const unsigned short &waitingTime);
    // Asigna el tiempo de bloqueo
    void setBlockedTime(const unsigned short &blockedTime);
    // Asigna el tiempo de respuesta
    void setResponseTime(const unsigned short &responseTime);
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
