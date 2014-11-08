#include <iostream>
#include "AgendaServiceInterface.h"

ServiceInterface service;
int main(int argc, char ** argv) {
    service.startAgenda();
    while (true) {
        if (service.updateAgenda()) {
            while (service.serverClient()) {
                service.handleclientRequirement();
            }
        }
    }
    service.quitAgenda();
    return 0;
}
