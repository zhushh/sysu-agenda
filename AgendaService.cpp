#include <iostream>
#include <list>
#include <algorithm>
#include <string>

#include "AgendaService.h"
#include "Storage.h"
#include "Meeting.h"
#include "Date.h"
#include "AGsocket.h"
using std::list;
using std::string;

AgendaService::AgendaService() {
    // storage_ = Storage::getInstance();
}
AgendaService::~AgendaService() {
    if (storage_ != NULL)
        quitAgenda();
}
void AgendaService::startAgenda(void) {
    storage_ = Storage::getInstance();
    agendaServerSocket_.start(true);
}
void AgendaService::quitAgenda(void) {
    storage_->sync();
    delete storage_;
    storage_ = NULL;
    agendaServerSocket_.quit();
}

bool AgendaService::userLogIn(std::string userName, std::string password) {
    list<User> temp = storage_->queryUser([&](const User& user) {
        return (userName == user.getName() && password == user.getPassword());
    });
    if (temp.empty()) {
        return false;
    } else {
        return true;
    }
}
bool AgendaService::userRegister(std::string userName, std::string password,
                  std::string email, std::string phone) {
    list<User> temp;
    temp = storage_->queryUser([&](const User& user) {
        return (userName == user.getName());
    });
    if (temp.empty()) {
        User u;
        u.setName(userName);
        u.setPassword(password);
        u.setEmail(email);
        u.setPhone(phone);
        storage_->createUser(u);
        return true;
    } else {
        return false;
    }
}
bool AgendaService::deleteUser(std::string userName, std::string password) {
    int count;
    count = storage_->deleteUser([&](const User& user) {
        return (user.getName() == userName
            && user.getPassword() == password);
    });
    if (count > 0) {
        // delete user meeting
        int temp = storage_->deleteMeeting([&](const Meeting& m) {
            if (m.getSponsor() == userName) {
                return true;
            } else {
                return false;
            }
        });
        return true;
    } else {
        return false;
    }
}
// a user can only delete itself
std::list<User> AgendaService::listAllUsers(void) {
    list<User> temp;
    temp = storage_->queryUser([](const User& user) {
        return true;
    });
    return temp;
}
bool AgendaService::createMeeting(std::string userName, std::string title,
                   std::string participator,
                   std::string startDate, std::string endDate) {
    Date d1 = Date::stringToDate(startDate), d2 = Date::stringToDate(endDate);
    // check that time format is valid
    if (!Date::isValid(d1)) {
        return false;
    }
    if (!Date::isValid(d2)) {
        return false;
    }
    if (d1 >= d2) {
        return false;
    }
    // check that the names is different
    if (userName == participator) {
        return false;
    }
    // check that user has registered
    if ((storage_->queryUser([userName](const User& u) {
        if (userName == u.getName()) {
            return true;
        } else {
            return false;
        }
    })).empty()) {
        return false;
    }
    // check that participartor has registered
    if ((storage_->queryUser([participator](const User& u) {
        if (participator == u.getName()) {
            return true;
        } else {
            return false;
        }
    })).empty()) {
        return false;
    }
    // check that title is identify
    if (!(storage_->queryMeeting([&](const Meeting& m) {
        if (m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    }).empty())) {
        return false;
    }
    // check that user and participator are all spare
    Meeting t(userName, participator, d1, d2, title);
    list<Meeting> sp = listAllMeetings(userName);
    for (auto p = sp.begin(); p != sp.end(); ++p) {
        if (p->getStartDate() > Date::stringToDate(startDate)
            && p->getStartDate() < Date::stringToDate(endDate)) {
            return false;
        }
        if (p->getStartDate() < Date::stringToDate(startDate)
            && p->getEndDate() > Date::stringToDate(startDate)) {
            return false;
        }
    }
    list<Meeting> pt = listAllMeetings(participator);
    for (auto p = pt.begin(); p != pt.end(); ++p) {
        if (p->getStartDate() > Date::stringToDate(startDate)
            && p->getStartDate() < Date::stringToDate(endDate)) {
            return false;
        }
        if (p->getStartDate() < Date::stringToDate(startDate)
            && p->getEndDate() > Date::stringToDate(startDate)) {
            return false;
        }
    }
    storage_->createMeeting(t);
    return true;
}
std::list<Meeting> AgendaService::meetingQuery(std::string userName, std::string title) {
    list<Meeting> temp;
    temp = storage_->queryMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        }
        if (m.getParticipator() == userName && m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    });
    return temp;
}
std::list<Meeting> AgendaService::meetingQuery(std::string userName, std::string startDate,
                                std::string endDate) {
    list<Meeting> temp;
    temp = storage_->queryMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName || m.getParticipator() == userName) {
            if (Date::stringToDate(startDate) > m.getEndDate()
                || Date::stringToDate(endDate) < m.getStartDate()) {
                return false;
            } else {
                return true;
            }
        } else {
            return false;
        }
    });
    return temp;
}
std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
    list<Meeting> temp;
    temp = storage_->queryMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName || m.getParticipator() == userName) {
            return true;
        } else {
            return false;
        }
    });
    return temp;
}
std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
    list<Meeting> temp;
    temp = storage_->queryMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName) {
            return true;
        } else {
            return false;
        }
    });
    return temp;
}
std::list<Meeting> AgendaService::listAllParticipateMeetings(std::string userName) {
    list<Meeting> temp;
    temp = storage_->queryMeeting([&](const Meeting& m) {
        if (m.getParticipator() == userName) {
            return true;
        } else {
            return false;
        }
    });
    return temp;
}
bool AgendaService::deleteMeeting(std::string userName, std::string title) {
    int temp;
    temp = storage_->deleteMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    });
    if (temp > 0) {
        return true;
    } else {
        return false;
    }
}
bool AgendaService::deleteAllMeetings(std::string userName) {
    int temp;
    temp = storage_->deleteMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName) {
            return true;
        } else {
            return false;
        }
    });
    if (temp > 0) {
        return true;
    } else {
        return false;
    }
}

// addition function
// change meeting time
bool AgendaService::changeStartTime(std::string userName, std::string title,
                     std::string newTime) {
    list<Meeting> m = storage_->queryMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    });
    if (m.empty()) {    // not meeting can be change
        return false;
    }
    // list<Meeting> backup = m;   // backup meeting
    int temp = storage_->deleteMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    });
    if (temp <= 0) {    // delete fail
        return false;
    }
    bool flag = true;
    for (auto p = m.begin(); p != m.end(); ++p) {
        if (!createMeeting(p->getSponsor(), p->getTitle(), p->getParticipator(),
            newTime, Date::dateToString(p->getEndDate()))) {
            createMeeting(p->getSponsor(), p->getTitle(), p->getParticipator(),
            Date::dateToString(p->getStartDate()), Date::dateToString(p->getEndDate()));
            flag = false;
        }
    }
    if (flag) {
        return true;
    } else {
        return false;
    }
}
bool AgendaService::changeEndTime(std::string userName, std::string title,
                   std::string newTime) {
    list<Meeting> m = storage_->queryMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    });
    if (m.empty()) {    // not meeting can be change
        return false;
    }
    // list<Meeting> backup = m;   // backup meeting
    int temp = storage_->deleteMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    });
    if (temp <= 0) {    // delete fail
        return false;
    }
    bool flag = true;
    for (auto p = m.begin(); p != m.end(); ++p) {
        if (!createMeeting(p->getSponsor(), p->getTitle(), p->getParticipator(),
            Date::dateToString(p->getStartDate()), newTime)) {
            createMeeting(p->getSponsor(), p->getTitle(), p->getParticipator(),
            Date::dateToString(p->getStartDate()), Date::dateToString(p->getEndDate()));
            flag = false;
        }
    }
    if (flag) {
        return true;
    } else {
        return false;
    }
}
bool AgendaService::changeMeetingTime(std::string userName, std::string title,
                       std::string newStartTime, std::string newEndTime) {
    // check that the userName is sponsor
    list<Meeting> m = storage_->queryMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    });
    if (m.empty()) {    // not meeting can be change
        return false;
    }
    // list<Meeting> backup = m;  // backup meeting list
    // check that deleting original Meeting is successful
    int temp = storage_->deleteMeeting([&](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        } else {
            return false;
        }
    });
    if (temp <= 0) {    // delete fail
        return false;
    }
    // change Meeting time
    bool flag = true;
    for (auto p = m.begin(); p != m.end(); ++p) {
        if (!createMeeting(p->getSponsor(), p->getTitle(), p->getParticipator(),
            newStartTime, newEndTime)) {
            createMeeting(p->getSponsor(), p->getTitle(), p->getParticipator(),
            Date::dateToString(p->getStartDate()), Date::dateToString(p->getEndDate()));
            flag = false;
        }
    }
    if (flag) {
        return true;
    } else {
        return false;
    }
}
// change user password
bool AgendaService::changeUserPassword(std::string userName, std::string oldPasswd,
                        std::string newPasswd) {
    list<User> u = storage_->queryUser([&](const User& user) {
        if (user.getName() == userName
            && user.getPassword() == oldPasswd) {
            return true;
        } else {
            return false;
        }
    });
    if (u.empty()) {
        return false;
    }
    int count;
    count = storage_->deleteUser([&](const User& user) {
        return (user.getName() == userName
            && user.getPassword() == oldPasswd);
    });
    if (count <= 0) {
        return false;
    } else {
        for (auto it = u.begin(); it != u.end(); ++it) {
            userRegister(it->getName(), newPasswd, it->getEmail(), it->getPhone());
        }
        return true;
    }
}
// query user by username
bool AgendaService::queryUser(std::string userName) {
    list<User> u = storage_->queryUser([&](const User& user) {
        if (user.getName() == userName) {
            return true;
        } else {
            return false;
        }
    });
    if (u.empty()) {
        return false;
    } else {
        return true;
    }
}


// provide the service for client
bool AgendaService::updateSocket() {
    if (storage_ != NULL) {
        storage_->sync();
    }
    return agendaServerSocket_.update();
}
void AgendaService::sendListOfMeeting(std::list<Meeting> temp) {
    for (auto itr = temp.begin(); itr != temp.end(); itr++) {
        agendaServerSocket_.sendSTR(itr->getSponsor());
        if (agendaServerSocket_.recvSTR() == "OK")
            agendaServerSocket_.sendSTR(itr->getParticipator());
        if (agendaServerSocket_.recvSTR() == "OK")
            agendaServerSocket_.sendSTR(Date::dateToString(itr->getStartDate()));
        if (agendaServerSocket_.recvSTR() == "OK")
            agendaServerSocket_.sendSTR(Date::dateToString(itr->getEndDate()));
        if (agendaServerSocket_.recvSTR() == "OK")
            agendaServerSocket_.sendSTR(itr->getTitle());
        if (agendaServerSocket_.recvSTR() == "OK")
            continue;
    }
    agendaServerSocket_.sendSTR("OK");   // send for telling the client the ending
}
void AgendaService::HandleClient() {
    std::string Operator = "";
    std::string userName, userPassword, email, phone;
    std::string title, participator, start_time, end_time;

    while ((Operator = agendaServerSocket_.recvSTR()) != "") {
        agendaServerSocket_.sendSTR("OK");
        if (Operator == "dc") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            userPassword = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");

            if (agendaServerSocket_.recvSTR() == "OK" && deleteUser(userName, userPassword)) {
                agendaServerSocket_.sendSTR("true");
            } else {
                agendaServerSocket_.sendSTR("false");
            }
        } else if (Operator == "lu") {
            list<User> temp = listAllUsers();
            for (auto itr = temp.begin(); itr != temp.end(); itr++) {
                agendaServerSocket_.sendSTR(itr->getName());
                if (agendaServerSocket_.recvSTR() == "OK")
                    agendaServerSocket_.sendSTR(itr->getEmail());
                if (agendaServerSocket_.recvSTR() == "OK")
                    agendaServerSocket_.sendSTR(itr->getPhone());
            }
            agendaServerSocket_.sendSTR("OK");
        } else if (Operator == "cm") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            title = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            participator = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            start_time = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            end_time = agendaServerSocket_.recvSTR();

            if (agendaServerSocket_.recvSTR() == "OK" && 
                createMeeting(userName, title, participator, start_time, end_time)) {
                agendaServerSocket_.sendSTR("true");
            } else {
                agendaServerSocket_.sendSTR("false");
            }
        } else if (Operator == "la") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            if (agendaServerSocket_.recvSTR() == "OK")
                sendListOfMeeting(listAllMeetings(userName));
        } else if (Operator == "las") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            if (agendaServerSocket_.recvSTR() == "OK")
                sendListOfMeeting(listAllSponsorMeetings(userName));
        } else if (Operator == "lap") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            if (agendaServerSocket_.recvSTR() == "OK")
                sendListOfMeeting(listAllParticipateMeetings(userName));
        } else if (Operator == "qm") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            title = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            if (agendaServerSocket_.recvSTR() == "OK")
                sendListOfMeeting(meetingQuery(userName, title));
        } else if (Operator == "qt") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            start_time = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            end_time = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            if (agendaServerSocket_.recvSTR() == "OK")
                sendListOfMeeting(meetingQuery(userName, start_time, end_time));
        } else if (Operator == "dm") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            title = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");

            if (agendaServerSocket_.recvSTR() == "OK" &&
                         deleteMeeting(userName, title)) {
                agendaServerSocket_.sendSTR("true");
            } else {
                agendaServerSocket_.sendSTR("false");
            }
        } else if (Operator == "da") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            if (agendaServerSocket_.recvSTR() == "OK" &&
                         deleteAllMeetings(userName)) {
                agendaServerSocket_.sendSTR("true");
            } else {
                agendaServerSocket_.sendSTR("false");
            }
        } else if (Operator == "l" || Operator == "login") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            userPassword = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            if (agendaServerSocket_.recvSTR() == "OK" &&
                 userLogIn(userName, userPassword)) {
                agendaServerSocket_.sendSTR("true");
            } else {
                agendaServerSocket_.sendSTR("false");
            }
        } else if (Operator == "r" || Operator == "register") {
            userName = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            userPassword = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            email = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            phone = agendaServerSocket_.recvSTR();
            agendaServerSocket_.sendSTR("OK");
            if (agendaServerSocket_.recvSTR() == "OK" &&
                userRegister(userName, userPassword, email, phone)) {
                agendaServerSocket_.sendSTR("true");
            } else {
                agendaServerSocket_.sendSTR("false");
            }
        } else if (Operator == "q" || Operator == "quit") {
            break;      // exit while
        } else {
            std::cout << "get undefined operator from user\n";
            agendaServerSocket_.sendSTR("false");
        }
    }
}
