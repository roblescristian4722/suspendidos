#include "lote.h"

Lote::Lote()
{}

Lote::~Lote()
{}

void Lote::capturarLote()
{
    Proceso aux;
    // Captura nombre
    capturarCampo("Ingrese el nombre del programador: ",
                  "ERROR: el nombre debe de contener al menos 3 caracteres "
                  "alfanuméricos (no debe de comenzar con un número)",
                  aux, &Proceso::setNombre);
    // Captura de operación
    capturarCampo("Ingrese la operación a realizar: ",
                  "ERROR: La operación ingresada no es válida. El formato "
                  "debe de ser \"int [+|-|/|%]\" int", aux,
                  &Proceso::setOperacion);
    // Captura de ID
    capturarCampo("Ingrese el ID del proceso: ",
                  "ERROR: el ID no es válido, debe de ser un número positivo "
                  "del 0 al 99999", aux, &Proceso::setID);
    // Captura de tiempo máximo
    capturarCampo("Ingrese el tiempo máximo estimado de ejecución: ",
                  "ERROR: el tiempo máximo de ejecución debe de ser un número "
                  "mayor a cero", aux, &Proceso::setTiempoMax);
    std::cout << "nombre: " << aux.getNombre() << std::endl
              << "id: " << aux.getID() << std::endl
              << "tiempo: " << aux.getTiempoMax() << std::endl
              << "opración: " << aux.getOperacion() << std::endl;
}

void Lote::capturarCampo(std::string msj, std::string msjError,
                         Proceso& proc,
                         bool(Proceso::*metodo)(const std::string&))
{
    std::string aux;
    while(1) {
        std::cout << colorText(VERDE, msj);
        std::getline(std::cin, aux);
        // Si el input es correcto rompemos el búcle infinito
        if ((proc.*metodo)(aux))
            break;
        rmLine();
        std::cout << colorText(ROJO, msjError, true) << std::endl;
    }
}
