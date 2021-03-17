#include "../define/proceso.h"

Proceso::Proceso(unsigned long id, std::string nombre, std::string operacion, 
                 unsigned long tiempoMax) : nombre(nombre), id(id),
                 tiempoMax(tiempoMax), operacion(operacion),
                 tiempoRes(tiempoMax), tiempoTrans(0) {}

Proceso::Proceso()
{ this->tiempoTrans = 0; }

Proceso::~Proceso() {}

const unsigned long& Proceso::getId() const
{ return this->id; }
const unsigned long& Proceso::getTiempoMax() const
{ return this->tiempoMax; }
const std::string& Proceso::getNombre() const
{ return this->nombre; }
const std::string& Proceso::getOperacion() const
{ return this->operacion; }
const unsigned int& Proceso::getLote() const
{ return this->lote; }
const long& Proceso::getResultado() const
{ return this->resultado; }
const unsigned long& Proceso::getTiempoTrans() const
{ return this->tiempoTrans; }
const unsigned long& Proceso::getTiempoRes() const
{ return this->tiempoRes; }

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

bool Proceso::setId(const std::string& id)
{
    std::regex validacion("[1-9][0-9]{0,4}");
    if (std::regex_match(id, validacion)){
        this->id = std::stoul(id);
        return true;
    }
    return false;
}

bool Proceso::setTiempoMax(const std::string& tiempoMax)
{
    std::regex validacion("[0-9]{1,4}");
    if (std::regex_match(tiempoMax, validacion)){
        if (stoul(tiempoMax) > 0 && stoul(tiempoMax) <= 3600) {
            this->tiempoTrans = 0;
            this->tiempoMax = stoul(tiempoMax);
            this->tiempoRes = stoul(tiempoMax);
            return true;
        }
    }
    return false;
}

void Proceso::setTiempoTrans(const unsigned long& tiempoTrans)
{ this->tiempoTrans = tiempoTrans; }
void Proceso::setTiempoRes(const unsigned long& tiempoRes)
{ this->tiempoRes = tiempoRes; }
void Proceso::setLote(const unsigned int &lote)
{ this->lote = lote; }

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
