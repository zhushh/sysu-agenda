#ifndef AGENDAUI_H
#define AGENDAUI_H

#include <iostream>
#include <string>
#include <list>

#ifdef AGENDASERVICE
#include "AgendaService.h"
#endif

#include "Meeting.h"
#include "Date.h"
#include "AGsocket.h"

class AgendaUI {
    public:
        AgendaUI();
        void OperationLoop(void);

    private:
        // task functions
        void startAgenda(void);
        std::string getOperation(void);
        bool executeOperation(std::string op);
        void userLogIn(void);
        void userRegister(void);
        void quitAgenda(void);
        void userLogOut(void);
        void deleteUser(void);
        void listAllUsers(void);
        void createMeeting(void);
        void listAllMeetings(void);
        void listAllSponsorMeetings(void);
        void listAllParticipateMeetings(void);
        void queryMeetingByTitle(void);
        void queryMeetingByTimeInterval(void);
        void deleteMeetingByTitle(void);
        void deleteAllMeetings(void);
        void printMeetings(std::list<Meeting> meetings);

        std::list<Meeting> getMeetings(void);
        bool updateSocket(void);
/*
        // change meeting time
        void changeStartTime(void);
        void changeEndTime(void);
        void changeMeetingTime(void);
        // void changUserName(void);
        // void sendEmail(void);
        void changeUserPassword(void);
        void printHistory(void);
*/
        // dates
        std::string userName_;
        std::string userPassword_;
        // AgendaService agendaService_;
        std::string readString(std::string);  // read non-empty string
        AGsocket agendaSocket_;
};

#endif

