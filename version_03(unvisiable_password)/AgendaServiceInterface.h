#ifndef AGENDASERVICEINTERFACE_H
#define AGENDASERVICEINTERFACE_H

#include <iostream>
#include <list>
#include <string>
#include <algorithm>

#include "Storage.h"
#include "Meeting.h"
#include "Date.h"
#include "AGsocket.h"
#include "AgendaService.h"

class ServiceInterface {
    public:
        ServiceInterface() {
            agendaService_ = new AgendaService;
        }
        ~ServiceInterface() {
            quitAgenda();
            delete agendaService_;
        }
        void startAgenda();
        bool updateAgenda();
        void quitAgenda();

        bool serverClient(void);
        void handleclientRequirement(void);
        void sendListOfMeeting(std::list<Meeting>);
        void readInfomationFromUser(std::string&);

        void userRegister(void);
        void userLogin(void);
        void userLogout(void);
        void deleteAccount(void);
        void sendAllUsers(void);
        void creatingMeeting(void);

        void sendAllMeetings(void);
        void sendAllParticipatorMeeting(void);
        void sendAllSponsorMeeting(void);
        void sendMeetingByTitle(void);
        void sendMeetingBytimeInterval(void);

        void deleteMeetingByTitle(void);
        void deleteAllMeetings(void);
        void changeMeetingStartTime(void);
        void changeMeetingEndTime(void);
        void changeMeetingTime(void);
        void changeUserPassword(void);

    private:
        AgendaService * agendaService_;
        AGsocket agendaServerSocket_;
        std::string Operator;
        std::string userName, userPassword, email, phone;
        std::string title, participator, start_time, end_time;
};

#endif
