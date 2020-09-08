/*
Copyright 2017-2020 Anthony Cohn-Richardby

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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

