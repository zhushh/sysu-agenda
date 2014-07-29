#ifndef AGENDASERVICE_H
#define AGENDASERVICE_H

#include "Storage.h"
#include <list>
#include <string>

class AgendaService {
    public:
        AgendaService();
        ~AgendaService();
        bool userLogIn(std::string userName, std::string password);
        bool userRegister(std::string userName, std::string password,
                          std::string email, std::string phone);
        bool deleteUser(std::string userName, std::string password);
        // a user can only delete itself
        std::list<User> listAllUsers(void);

        bool createMeeting(std::string userName, std::string title,
                           std::string participator,
                           std::string startDate, std::string endDate);
        std::list<Meeting> meetingQuery(std::string userName, std::string title);
        std::list<Meeting> meetingQuery(std::string userName, std::string startDate,
                                        std::string endDate);
        std::list<Meeting> listAllMeetings(std::string userName);
        std::list<Meeting> listAllSponsorMeetings(std::string userName);
        std::list<Meeting> listAllParticipateMeetings(std::string userName);
        bool deleteMeeting(std::string userName, std::string title);
        bool deleteAllMeetings(std::string userName);

        void startAgenda(void);
        void quitAgenda(void);

        // addition function
        // change meeting time
        bool changeStartTime(std::string userName, std::string title,
                             std::string newTime);
        bool changeEndTime(std::string userName, std::string title,
                           std::string newTime);
        bool changeMeetingTime(std::string userName, std::string title,
                               std::string newStartTime, std::string newEndTime);
        bool changeUserPassword(std::string userName, std::string oldPasswd,
                                std::string newPasswd);
        bool queryUser(std::string userName);   // determine whether the username exists

    private:
        Storage *storage_;
};

#endif
