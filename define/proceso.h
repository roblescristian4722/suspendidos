#ifndef PROCESO_H
#define PROCESO_H
#include <iostream>
#include <regex>
#include "cursor.h"

class Proceso
{
private:
    std::string nombre;
    std::string operacion;
    long resultado;
    unsigned long ID;
    unsigned long tiempoMax;
    unsigned long tiempoRes;
    unsigned long tiempoTrans;
public:
    Proceso();
    Proceso(std::string nombre, std::string operacion, unsigned long ID,
            unsigned long tiempoMax);
    ~Proceso();
    
    // Obtiene el nombre del programador del proceso
    const std::string& getNombre() const;
    // Obtiene el tipo de operación a realizar en el proceso
    const std::string& getOperacion() const;
    // Obtiene el ID único del proceso
    const unsigned long& getID() const;
    // Obtiene el tiempo máximo estimado de ejecución del proceso (en segundos)
    const unsigned long& getTiempoMax() const;
    // Obtiene el resultado de la operación 
    const long& getResultado() const;
    // Obtiene el tiempo restante
    const unsigned long& getTiempoRes() const;
    // Obtiene el tiepo transcurrido
    const unsigned long& getTiempoTrans() const;
    
    // Genera el resultado de la operación
    void calculate();
    // Asigna el tiempo restante
    void setTiempoRes(const unsigned long& tiempoRes);
    // Asigna el tiepo transcurrido
    void setTiempoTrans(const unsigned long& tiempoTrans);
    // Asigna el nombre del programador del proceso
    bool setNombre(const std::string& nombre);
    // Asigna el tipo de operacion a realizar en el proceso
    bool setOperacion(const std::string& operacion);
    // Asigna el ID único al proceso
    bool setID(const std::string& ID);
    // Asigna el tiempo máximo estimado de ejecucón del proceso (en segundos)
    bool setTiempoMax(const std::string& tiempoMax);
};

#endif // PROCESO_H
