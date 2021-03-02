#include "../define/proceso.h"

Proceso::Proceso(std::string nombre, std::string operacion, unsigned long ID, 
                 unsigned long tiempoMax) : nombre(nombre), ID(ID),
                 tiempoMax(tiempoMax), operacion(operacion) {}

Proceso::Proceso(std::string tmp): operacion(tmp) 
{ this->resultado = 0; }

Proceso::~Proceso() {}

const unsigned long& Proceso::getID() const
{ return this->ID; }
const unsigned long& Proceso::getTiempoMax() const
{ return this->tiempoMax; }
const std::string& Proceso::getNombre() const
{ return this->nombre; }
const std::string& Proceso::getOperacion() const
{ return this->operacion; }

//Setters validados
bool Proceso::setNombre(const std::string& nombre)
{
    std::regex validacion("[a-zA-Z][a-zA-Z0-9]{2,}");
    if (std::regex_match(nombre, validacion)){
        this->nombre = nombre;
        return true;
    }
    return false;
}

bool Proceso::setOperacion(const std::string& operacion)
{
    std::regex validacion("(-?\\s*\\d+\\s*[-|+|*|/|%]\\s*-?\\s*\\d+)");
    std::regex sobreCero("(-?\\s*\\d+\\s*[/|%]\\s*-?\\s*0)");
    if (std::regex_match(operacion, validacion) && 
        !std::regex_match(operacion, sobreCero)) {
        this->operacion = operacion;
        return true;
    }
    return false;
}

bool Proceso::setID(const std::string& ID, std::map<std::string, bool>* IDs)
{
    std::regex validacion("[1-9][0-9]{0,4}");
    if (std::regex_match(ID, validacion)){
        if (IDs->find(ID) == IDs->end()) {
            (*IDs)[ID] = true;
            this->ID = std::stoul(ID);
            return true;
        }
    }
    return false;
}
bool Proceso::setTiempoMax(const std::string& tiempoMax)
{
    std::regex validacion("[0-9]{1,4}");
    if (std::regex_match(tiempoMax, validacion)){
        if (stoul(tiempoMax) > 0 && stoul(tiempoMax) <= 3600) {
            this->tiempoMax = stoul(tiempoMax);
            return true;
        }
    }
    return false;
}

const long& Proceso::getResultado() const
{ return this->resultado; }

void Proceso::calculate()
{
    bool first = false;
    long num1 = 0;
    long num2 = 0;
    char op = ' ';
    std::string aux = "";
    std::string nextChar = "";
    std::regex opDis("[-|/|+|*|%]");
    
    for (size_t i = 0; i < this->operacion.size(); ++i) {
        if (this->operacion[i] != ' ')
            aux += this->operacion[i];
        if (aux != "") {
            // símbolo de operación
            if (std::regex_match(aux, opDis) && first && op == ' ') {
                op = aux[0];
                aux = "";
            }
            // No es el último caracter de la cadena
            else if (i < this->operacion.size() - 1) {
                nextChar = this->operacion[i + 1];
                // first number
                if (std::regex_match(nextChar, opDis) ||
                     (nextChar == " " && !first)) {
                    first = true;
                    num1 = stol(aux);
                    aux = "";
                }
            }
            // es el último caracter = segundo número
            else
                num2 = stol(aux);
        }
    }
    // second number
    if (aux != "")
        num2 = stol(aux);

    switch (op) {
        case '/':
            this->resultado = num1 / num2;
        break;
        case '%':
            this->resultado = num1 % num2;
        break;
        case '*':
            this->resultado = num1 * num2;
        break;
        case '+':
            this->resultado = num1 + num2;
        break;
        case '-':
            this->resultado = num1 - num2;
        break;
    }
}
