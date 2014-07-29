#include "AgendaUI.h"
#include <iostream>

AgendaUI aui;
void print_info() {
    std::cout << "Hello, Welcome using Agenda!" << std::endl
         << "This Agenda was made by shuhuang" << std::endl
         << "Version: 14-05-agd" << std::endl
         << "Email: zsh5318@gmail.com\n" << std::endl;
}
int main() {
    print_info();
    aui.OperationLoop();
    return 0;
}
