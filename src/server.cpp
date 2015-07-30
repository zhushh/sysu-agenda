/*************************************************************************
 > File Name: server.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Tue 21 Jul 2015 11:48:52 AM CST
 ************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>

//#include "AgendaUI.h"
#include "../headers/AGsocket.h"
#include "../headers/AgendaService.h"

void error(const char* err_mesg) {
    perror(err_mesg);
    exit(1);
}

void serverFunc(AGsocket &ser);

int main() {
    int pid;
    AGsocket server;
    server.start(true);
    while (true) {
        if (server.update()) {
            pid = fork();
            if (pid < 0) {
                error("Fork function fail!");
            } else if (pid == 0) {      // child process
                server.quit();
                serverFunc(server);
                exit(0);
            }
        }
    }
    return 0;
}

void serverFunc(AGsocket &ser) {
    AgendaService ags;      // control server logic data
    std::string userName_ = "";          // store user name on server
    std::string userPassword_ = "";      // store user password on server
    std::cout << "Start Server function~~ " << std::endl;

    ags.startAgenda();
    while (true) {
        std::string op = ser.recvSTR();
        if (!ser.sendSTR("GET")) {
            continue;  // error send data
        }
        std::cout << "Server get client request operation: " << op << std::endl;
        if (op == "q") {
            break;
        }
        if (!userName_.empty()) {
            if (op == "o") {
                // userLogOut();
                userName_ = "";
                userPassword_ = "";
            } else if (op == "dc") {
                //deleteUser();
            } else if (op == "lu") {
                //listAllUsers();
            } else if (op == "cm") {
                //createMeeting();
            } else if (op == "la") {
                //listAllMeetings();
            } else if (op == "las") {
                //listAllSponsorMeetings();
            } else if (op == "lap") {
                //listAllParticipateMeetings();
            } else if (op == "qm") {
                //queryMeetingByTitle();
            } else if (op == "qt") {
                //queryMeetingByTimeInterval();
            } else if (op == "dm") {
                //deleteMeetingByTitle();
            } else if (op == "da") {
                //deleteAllMeetings();
            }
        } else {
            if (op == "l" || op == "login") {
                std::cout << "Prepare for client login" << std::endl;
                userName_ = ser.recvSTR();
                if (!ser.sendSTR("Get")) {
                    std::cout << "Server send data to client fail" << std::endl;
                    break;
                }
                userPassword_ = ser.recvSTR();

                std::cout << "Server receive data from client: " << std::endl;
                std::cout << "userName_ = " << userName_ << std::endl;
                std::cout << "userPassword_ = " << userPassword_ << std::endl;

                if (ags.userLogIn(userName_, userPassword_)) {
                    ser.sendSTR("true");
                } else {
                    ser.sendSTR("false");
                }
            } else if (op == "r" || op == "register") {
                //userRegister();
                std::string info[4];
                for (int i = 0; i < 4; i++) {
                    info[i] = ser.recvSTR();
                    if (i < 3) {
                        if (!ser.sendSTR("Get")) {
                            std::cout << "Server send 'Get' fail in register!" << std::endl;
                            return ;
                        }
                    }
                }
                if (ags.userRegister(info[0], info[1], info[2], info[3])) {
                    ser.sendSTR("true");
                } else {
                    ser.sendSTR("false");
                }
            }
        }
    }
    std::cout << "Current User size == " << ags.listAllUsers().size() << std::endl;
    ags.quitAgenda();
}
