#ifndef PROCESO_H
#define PROCESO_H
#include <iostream>
#include <regex>

class Proceso
{
private:
    std::string nombre;
    std::string operacion;
    unsigned long ID;
    unsigned long tiempoMax;
public:
    Proceso(std::string nombre, std::string operacion, unsigned long ID,
            unsigned long tiempoMax);
    Proceso();
    ~Proceso();
    
    // Asigna el nombre del programador del proceso
    const std::string& getNombre() const;
    // Asigna el tipo de operacion a realizar en el proceso
    const std::string& getOperacion() const;
    // Asigna el ID único al proceso
    const unsigned long& getID() const;
    // Asigna el tiempo máximo estimado de ejecucón del proceso (en segundos)
    const unsigned long& getTiempoMax() const;
    // Obtiene el nombre del programador del proceso
    bool setNombre(const std::string& nombre);
    // Obtiene el tipo de operación a realizar en el proceso
    bool setOperacion(const std::string& operacion);
    // Obtiene el ID único del proceso
    bool setID(const std::string& ID);
    // Obtiene el tiempo máximo estimado de ejecución del proceso (en segundos)
    bool setTiempoMax(const std::string& tiempoMax);
};

#endif // PROCESO_H
