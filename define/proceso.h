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
public:
    Proceso(std::string nombre, std::string operacion, unsigned long ID,
            unsigned long tiempoMax);
    Proceso(std::string tmp = "3+3");
    ~Proceso();
    
    // Obtiene el nombre del programador del proceso
    const std::string& getNombre() const;
    // Obtiene el tipo de operación a realizar en el proceso
    const std::string& getOperacion() const;
    // Obtiene el ID único del proceso
    const unsigned long& getID() const;
    // Obtiene el tiempo máximo estimado de ejecución del proceso (en segundos)
    const unsigned long& getTiempoMax() const;
    const long& getResultado() const;
    // Obtiene el resultado de la operación 
    
    // Genera el resultado de la operación
    void calculate();
    // Asigna el nombre del programador del proceso
    bool setNombre(const std::string& nombre);
    // Asigna el tipo de operacion a realizar en el proceso
    bool setOperacion(const std::string& operacion);
    // Asigna el ID único al proceso
    bool setID(const std::string& ID,
               std::map<std::string, bool>* IDs);
    // Asigna el tiempo máximo estimado de ejecucón del proceso (en segundos)
    bool setTiempoMax(const std::string& tiempoMax);
};

#endif // PROCESO_H
