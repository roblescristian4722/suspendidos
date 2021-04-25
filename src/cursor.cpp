#include "../define/cursor.h"

Frame::Frame(int x, int y, int w, int h, char color, bool foreground,
               std::string simbol): heightFrame(h), widthFrame(w), xFrame(x),
                                    yFrame(y), foreground(foreground),
                                    contentColor(BLANCO), simbol(simbol),
                                    color(color)
{
    drawFrame(x, y, w, h, color, foreground, simbol);
    this->xPos = x + 1;
    this->yPos = y + 1;
}

Frame::~Frame(){}

void Cursor::rmPrint(const int &x, const int &y, const std::string &msg,
                     const int &rm, const char &color)
{
    gotoxy(x, y);
    rmLine(rm);
    std::cout << colorText(color, msg);
}

void Frame::setFrame(int x, int y, int w, int h, char color, bool foreground,
               std::string simbol)
{
    this->heightFrame = h;
    this->widthFrame = w;
    this->xFrame = x;
    this->yFrame = y;
    this->foreground = foreground;
    this->simbol = simbol;
    this->color = color;
    drawFrame(x, y, w, h, color, foreground, simbol);
    this->xPos = x + 1;
    this->yPos = y + 1;
}

void Cursor::gotoxy(int x,int y)
{ printf("%c[%d;%df",0x1B,y,x); }

void Cursor::clrscr()
{ system(CLRSCR); }

void Cursor::hideCursor()
{ printf("\e[?25l"); }

void Cursor::showCursor()
{ printf("\e[?25h"); }

std::string Cursor::colorText(unsigned char color, std::string msg,
                              bool foreground)
{
    std::string aux;
    // foreground ANSI: "\033[0(fondo negro);3{color}m{msg}\033[0m"
    if (foreground) {
        aux = "\033[0;3";
        aux += color;
    }
    // background ANSI: "\033[4{color fondo}m\033[1;30(color foreground negro)m
    //              {msg}\033[0m"
    else {
        aux = "\033[4";
        aux += color;
        aux += "m\033[1;30";
    }
    aux += 'm';
    aux += msg;
    aux += "\033[0m";
    return aux;
}

void Cursor::rmLine(unsigned int n)
{
    while (n--) {
        // Borra la línea actual
        std::cout << "\033[2K";
        // Regresa a la línea anterior (sube 1 línea)
        std::cout << "\033[1A";
    }
}

void Cursor::drawXLine(int x, int y, int w, char color, bool foreground,
               std::string caracter)
{
    int i;
    for (i = 0; i < w; ++i) {
        gotoxy(x + i, y);
        std::cout << colorText(color, caracter);
    }
    gotoxy(i, y);
    std::cout.flush();
}

void Cursor::drawYLine(int x, int y, int h, char color, bool foreground,
               std::string caracter)
{
    int i;
    for (i = 0; i < h; ++i) {
        gotoxy(x, y + i);
        std::cout << colorText(color, caracter);
    }
    gotoxy(x, i);
    std::cout.flush();
}

void Frame::drawFrame(int x, int y, int w, int h, char color, bool foreground,
               std::string caracter)
{
    drawXLine(x, y, w, color, foreground, caracter);
    drawYLine(x, y, h, color, foreground, caracter);
    drawYLine(x + w - 1, y, h, color, foreground, caracter);
    drawXLine(x, y + h - 1, w, color, foreground, caracter);
    gotoxy(x + 1, y + 1);
}

void Frame::print(const std::string& msj, char color, bool newl,
                  unsigned int fill)
{
    std::string aux;
    unsigned int times = msj.size() / (this->widthFrame - 2);

    for (size_t i = 0; i < msj.size(); ++i) {
        gotoxy(this->xPos, this->yPos);
        aux = msj[i];
        std::cout << colorText(color, aux, this->foreground);
        if (++this->xPos == this->xFrame + this->widthFrame - 1) {
            this->xPos = xFrame + 1;
            ++this->yPos;
            gotoxy(this->xPos, this->yPos);
        }
    }
    if (fill)
        fillContent(msj, fill);
    if (newl) {
        this->xPos = xFrame + 1;
        ++this->yPos;
        gotoxy(this->xPos, this->yPos);
    }
    std::cout.flush();
}

void Frame::printNum(const int& msj, char color, bool newl)
{
    gotoxy(this->xPos, this->yPos);
    std::cout << msj;
    if (newl) {
        this->xPos = xFrame + 1;
        ++this->yPos;
        gotoxy(this->xPos, this->yPos);
    }
}

void Frame::rmContentLine()
{
    this->xPos = this->xFrame + 1;
    drawXLine(this->xPos, this->yPos, this->widthFrame - 2, BLANCO, false, " ");
    --this->yPos;
    gotoxy(this->xPos, this->yPos);
}

void Frame::rmContent(bool wipe)
{
    while (this->yPos != this->yFrame)
        rmContentLine();
    this->yPos = yFrame + 1;
}

void Frame::fillContent(const std::string &msj, unsigned int n)
{
    if (msj.size() < n)
        this->xPos = xPos + (n - msj.size());
    gotoxy(this->xPos, this->yPos);
}

void Frame::update(const std::string &msj, char color,
                   bool newl, unsigned int fill)
{
    rmContent();
    print(msj, color, newl, fill);
}