#if !defined(_WIN32) || !defined(_WIN64)

#ifndef KBHIT_LINUX_H
#define KBHIT_LINUX_H
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>

int kbhit();
void set_echo();
void unset_echo();

#endif // KBHIT_LINUX_H

#endif
