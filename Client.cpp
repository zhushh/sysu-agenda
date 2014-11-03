#include "AgendaUI.h"
#include <iostream>

AgendaUI aui;
void print_info() {
    std::cout << "Hello, Welcome use Agenda!" << std::endl
         << "This Agenda was made by shuhuang" << std::endl
         << "Version: 14-02-agd" << std::endl
         << "Email: zsh5318@gmail.com\n" << std::endl;
}
int main(int argc, char ** argv) {
    print_info();
    aui.OperationLoop();
    return 0;
}

