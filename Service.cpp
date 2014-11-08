#include <iostream>
#include "AgendaService.h"

AgendaService service;
int main(int argc, char ** argv) {
    service.startAgenda();
    while (true) {
        if (service.updateSocket())
            service.HandleClient();
        // int chioce;
        // std::cin >> chioce;
        // if (chioce == 0)
        //     break;
    }
    service.quitAgenda();
    return 0;
}
