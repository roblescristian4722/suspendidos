#include "../define/process.h"

Process::Process(unsigned long id, std::string name, std::string op, 
                 unsigned short maxTime) : name(name), id(id),
                 maxTime(maxTime), op(op),
                 remTime(maxTime), serviceTime(0) {}

Process::Process()
{
    this->serviceTime = 0;
    this->blockedTime = 0;
    this->arrivalTime = 0;
    this->finishTime = 0;
    this->maxTime = 0;
    this->remTime = 0;
    this->returnTime = 0;
    this->op = "0";
    this->responseTime = -1;
}

Process::~Process() {}

const unsigned long& Process::getId() const
{ return this->id; }
const unsigned short& Process::getMaxTime() const
{ return this->maxTime; }
const std::string& Process::getName() const
{ return this->name; }
const std::string& Process::getOp() const
{ return this->op; }
const std::string& Process::getResult() const
{ return this->result; }
const unsigned short& Process::getServiceTime() const
{ return this->serviceTime; }
const unsigned short& Process::getRemTime() const
{ return this->remTime; }
const unsigned short &Process::getArrivalTime() const
{ return this->arrivalTime; }
const unsigned short &Process::getFinishTime()const
{ return this->finishTime; }
const unsigned short &Process::getReturnTime() const
{ return this->returnTime; }
const unsigned short &Process::getWaitingTime() const
{ return this->waitingTime; }
const unsigned short &Process::getBlockedTime() const
{ return this->blockedTime; }
const short &Process::getResponseTime() const
{ return this->responseTime; }

//Setters validados
bool Process::setName(const std::string& name)
{
    std::regex validacion("[a-zA-Z][a-zA-Z0-9]{2,}");
    if (std::regex_match(name, validacion)){
        this->name = name;
        return true;
    }
    return false;
}

bool Process::setOp(const std::string& op)
{
    std::regex validacion("(-?\\s*\\d+\\s*[-|+|*|/|%]\\s*-?\\s*\\d+)");
    std::regex sobreCero("(-?\\s*\\d+\\s*[/|%]\\s*-?\\s*0)");
    if (std::regex_match(op, validacion) && 
        !std::regex_match(op, sobreCero)) {
        this->op = op;
        return true;
    }
    return false;
}

bool Process::setId(const std::string& id)
{
    std::regex validacion("[0-9][0-9]{0,4}");
    if (std::regex_match(id, validacion)){
        this->id = std::stoul(id);
        return true;
    }
    return false;
}

bool Process::setMaxTime(const std::string& maxTime)
{
    std::regex validacion("[0-9]{1,4}");
    if (std::regex_match(maxTime, validacion)){
        if (stoul(maxTime) > 0 && stoul(maxTime) <= 3600) {
            this->serviceTime = 0;
            this->maxTime = stoul(maxTime);
            this->remTime = stoul(maxTime);
            return true;
        }
    }
    return false;
}

void Process::setServiceTime(const unsigned short &serviceTime)
{ this->serviceTime = serviceTime; }
void Process::setRemTime(const unsigned short& remTime)
{ this->remTime = remTime; }
void Process::setResult(const std::string &result)
{ this->result = result; }
void Process::setArrivalTime(const unsigned short &arrivalTime)
{ this->arrivalTime = arrivalTime; }
void Process::setFinishTime(const unsigned short &finishTime)
{ this->finishTime = finishTime; }
void Process::setReturnTime(const unsigned short &returnTime)
{ this->returnTime = returnTime; }
void Process::setWaitingTime(const unsigned short &waitingTime)
{ this->waitingTime = waitingTime; }
void Process::setBlockedTime(const unsigned short &blockedTime)
{ this->blockedTime = blockedTime; }
void Process::setResponseTime(const unsigned short &responseTime)
{ this->responseTime = responseTime; }

void Process::calculate()
{
    bool first = false;
    long num1 = 0;
    long num2 = 0;
    char op = ' ';
    std::string aux = "";
    std::string nextChar = "";
    std::regex opDis("[-|/|+|*|%]");
    
    for (size_t i = 0; i < this->op.size(); ++i) {
        if (this->op[i] != ' ')
            aux += this->op[i];
        if (aux != "") {
            // símbolo de operación
            if (std::regex_match(aux, opDis) && first && op == ' ') {
                op = aux[0];
                aux = "";
            }
            // No es el último caracter de la cadena
            else if (i < this->op.size() - 1) {
                nextChar = this->op[i + 1];
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
            this->result = std::to_string(num1 / num2);
        break;
        case '%':
            this->result = std::to_string(num1 % num2);
        break;
        case '*':
            this->result = std::to_string(num1 * num2);
        break;
        case '+':
            this->result = std::to_string(num1 + num2);
        break;
        case '-':
            this->result = std::to_string(num1 - num2);
        break;
    }
}
