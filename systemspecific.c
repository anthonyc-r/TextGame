#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef linux
#include <termios.h>
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#include "systemspecific.h"


void
systemspecific_clear()
{
#ifdef linux
    system("clear");
#endif
#ifdef _WIN32
	system("cls");
#endif
}

char
systemspecific_getch()
{
    char character=0;
#ifdef linux
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&character,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
#endif
#ifdef _WIN32
    character = getch();
#endif
    return character;
}

void 
systemspecific_sleep(int milliseconds)
{
#ifdef linux
    usleep(1000 * milliseconds);
#endif
#ifdef _WIN32
    Sleep(milliseconds);
#endif
}

