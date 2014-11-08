#include <unistd.h>
#include <termio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

#define passLength 100

void read_password(std::string& password, std::string prompt) {
    struct termio tio, tin;
    char* ptr = (char*)malloc(passLength);
    char* b = ptr;

    ioctl(0, TCGETA, &tio);
    tin = tio;
    tin.c_lflag &= ~ECHO; /* turn off ECHO */
    /*
    * Set the new modes. Again we ignore return
    * values.
    */
    ioctl(0, TCSETA, &tin);

    char selected;
    int order = 0;
    cout << prompt;
    do {
        selected = fgetc(stdin);
        if ((selected == '\b') && (order > 0))
        {
            order--;
            ptr--;
            *ptr = '\0';
        } else if ((selected != '\n') && (selected != '\r') && (selected != '\b')) {
            *ptr++ = selected;
            order++;
            fflush(stdout);
        }
    } while ((selected!='\n')&&(selected!='\r')&&(order < passLength));
    ptr[order] = '\0';
    /*
    * Reset the old tty modes.
    */
    ioctl(0, TCSETA, &tio);
    password = b;
    cout << endl;
    free(b);
}
