#include <iostream>
#include <list>
#include <string>
#include <algorithm>

#include "AgendaServiceInterface.h"
#include "Storage.h"
#include "Meeting.h"
#include "Date.h"
#include "AGsocket.h"
#include "AgendaService.h"
using std::list;
using std::string;
using std::cout;
using std::endl;

void ServiceInterface::startAgenda(void) {
    agendaServerSocket_.start(true);
    agendaService_->startAgenda();
}
bool ServiceInterface::updateAgenda() {
    if (agendaServerSocket_.update())
        return true;
    else
        return false;
}
void ServiceInterface::quitAgenda(void) {
   agendaServerSocket_.quit();
   agendaService_->quitAgenda();
}

bool ServiceInterface::serverClient(void) {
    readInfomationFromUser(Operator);
    if (Operator == "q" || Operator == "quit") {
        agendaService_->updateAgenda();
        return false;
    } else {
        return true;
    }
}
/// read info from users
void ServiceInterface::readInfomationFromUser(std::string& receive) {
    if ((receive = agendaServerSocket_.recvSTR()) != "") {
        agendaServerSocket_.sendSTR("OK");
    }
}
/// handler
void ServiceInterface::handleclientRequirement(void) {
    if (Operator == "r" || Operator == "register") { userRegister(); }
    if (Operator == "l" || Operator == "login") { userLogin(); }
    if (Operator == "o") { userLogout(); }
    if (Operator == "dc") { deleteAccount(); }
    if (Operator == "lu") { sendAllUsers(); }
    if (Operator == "cm") { creatingMeeting(); }
    if (Operator == "la") { sendAllMeetings(); }
    if (Operator == "lap") { sendAllParticipatorMeeting(); }
    if (Operator == "las") { sendAllSponsorMeeting(); }
    if (Operator == "qm") { sendMeetingByTitle(); }
    if (Operator == "qt") { sendMeetingBytimeInterval(); }
    if (Operator == "dm") { deleteMeetingByTitle(); }
    if (Operator == "da") { deleteAllMeetings(); }
    if (Operator == "cst") { changeMeetingStartTime(); }
    if (Operator == "cet") { changeMeetingEndTime(); }
    if (Operator == "ct") { changeMeetingTime(); }
    if (Operator == "cp") { changeUserPassword(); }
}
/// send the meeting lists to user
void ServiceInterface::sendListOfMeeting(std::list<Meeting> temp) {
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

void ServiceInterface::userRegister(void) {       // register account
    readInfomationFromUser(userName);           if (userName == "q") return;
    readInfomationFromUser(userPassword);       if (userPassword == "q") return;
    readInfomationFromUser(email);              if (email == "q") return;
    readInfomationFromUser(phone);              if (phone == "q") return;

    if (agendaServerSocket_.recvSTR() == "OK" &&
        agendaService_->userRegister(userName, userPassword, email, phone)) {
        agendaServerSocket_.sendSTR("true");
    } else {
        agendaServerSocket_.sendSTR("false");
    }
}
void ServiceInterface::userLogin(void) {          // login account
    readInfomationFromUser(userName);
    readInfomationFromUser(userPassword);

    if (agendaServerSocket_.recvSTR() == "OK" &&
        agendaService_->userLogIn(userName, userPassword)) {
        agendaServerSocket_.sendSTR("true");
    } else {
        agendaServerSocket_.sendSTR("false");
    }
}
void ServiceInterface::userLogout(void) {
    cout << "user logout" << endl;
}
void ServiceInterface::deleteAccount(void) {      // user delete account
    readInfomationFromUser(userName);
    readInfomationFromUser(userPassword);

    if (agendaServerSocket_.recvSTR() == "OK" &&
        agendaService_->deleteUser(userName, userPassword)) {
        agendaServerSocket_.sendSTR("true");
    } else {
        agendaServerSocket_.sendSTR("false");
    }
}
void ServiceInterface::sendAllUsers(void) {       // list all user
    list<User> temp = agendaService_->listAllUsers();
    for (auto itr = temp.begin(); itr != temp.end(); itr++) {
        if (agendaServerSocket_.recvSTR() == "OK")
            agendaServerSocket_.sendSTR(itr->getName());
        if (agendaServerSocket_.recvSTR() == "OK")
            agendaServerSocket_.sendSTR(itr->getEmail());
        if (agendaServerSocket_.recvSTR() == "OK")
            agendaServerSocket_.sendSTR(itr->getPhone());
    }
    if (agendaServerSocket_.recvSTR() == "OK")
        agendaServerSocket_.sendSTR("OK");
}
void ServiceInterface::creatingMeeting(void) {    // creating a meeting
    readInfomationFromUser(userName);           if (userName == "q") return;
    readInfomationFromUser(title);              if (title == "q") return;
    readInfomationFromUser(participator);       if (participator == "q") return;
    readInfomationFromUser(start_time);         if (start_time == "q") return;
    readInfomationFromUser(end_time);           if (end_time == "q") return;

    if (agendaServerSocket_.recvSTR() == "OK")
        cout << "receive finished" << endl;

    if (agendaService_->createMeeting(userName, title, participator, start_time, end_time)) {
        agendaServerSocket_.sendSTR("true");
    } else {
        agendaServerSocket_.sendSTR("false");
    }
}

void ServiceInterface::sendAllMeetings(void) {    // list all meeting
    readInfomationFromUser(userName);
    if (agendaServerSocket_.recvSTR() == "OK")
        sendListOfMeeting(agendaService_->listAllMeetings(userName));
}
void ServiceInterface::sendAllParticipatorMeeting(void) {     // list all participator meeting
    readInfomationFromUser(userName);
    if (agendaServerSocket_.recvSTR() == "OK")
        sendListOfMeeting(agendaService_->listAllParticipateMeetings(userName));
}
void ServiceInterface::sendAllSponsorMeeting(void) {          // list all sponsor meeting
    readInfomationFromUser(userName);
    if (agendaServerSocket_.recvSTR() == "OK")
        sendListOfMeeting(agendaService_->listAllSponsorMeetings(userName));
}
void ServiceInterface::sendMeetingByTitle(void) {             // query meeting by title
    readInfomationFromUser(userName);
    readInfomationFromUser(title);
    if (agendaServerSocket_.recvSTR() == "OK")
        sendListOfMeeting(agendaService_->meetingQuery(userName, title));
}
void ServiceInterface::sendMeetingBytimeInterval(void) {      // query meeting by timeInterval
    readInfomationFromUser(userName);
    readInfomationFromUser(start_time);
    readInfomationFromUser(end_time);
    if (agendaServerSocket_.recvSTR() == "OK")
        sendListOfMeeting(agendaService_->meetingQuery(userName, start_time, end_time));
}

void ServiceInterface::deleteMeetingByTitle(void) {           // delete meeting by title
    readInfomationFromUser(userName);
    readInfomationFromUser(title);
    if (agendaServerSocket_.recvSTR() == "OK" &&
        agendaService_->deleteMeeting(userName, title)) {
        agendaServerSocket_.sendSTR("true");
    } else {
        agendaServerSocket_.sendSTR("false");
    }
}
void ServiceInterface::deleteAllMeetings(void) {              // delete all meetings
    readInfomationFromUser(userName);
    if (agendaServerSocket_.recvSTR() == "OK" &&
        agendaService_->deleteAllMeetings(userName)) {
        agendaServerSocket_.sendSTR("true");
    } else {
        agendaServerSocket_.sendSTR("false");
    }
}
void ServiceInterface::changeMeetingStartTime(void) {         // change meeting start time
    readInfomationFromUser(userName);
    readInfomationFromUser(title);
    readInfomationFromUser(start_time);

    if (agendaServerSocket_.recvSTR() == "OK") {
        if (agendaService_->changeStartTime(userName, title, start_time)) {
            agendaServerSocket_.sendSTR("true");
        } else {
            agendaServerSocket_.sendSTR("false");
        }
    }
}
void ServiceInterface::changeMeetingEndTime(void) {           // change meeting end time
    readInfomationFromUser(userName);
    readInfomationFromUser(title);
    readInfomationFromUser(start_time);

    if (agendaServerSocket_.recvSTR() == "OK") {
        if (agendaService_->changeEndTime(userName, title, start_time)) {
            agendaServerSocket_.sendSTR("true");
        } else {
            agendaServerSocket_.sendSTR("false");
        }
    }
}
void ServiceInterface::changeMeetingTime(void) {              // change meeting time
    readInfomationFromUser(userName);
    readInfomationFromUser(title);
    readInfomationFromUser(start_time);
    readInfomationFromUser(end_time);

    if (agendaServerSocket_.recvSTR() == "OK") {
        if (agendaService_->changeMeetingTime(userName, title, start_time, end_time)) {
            agendaServerSocket_.sendSTR("true");
        } else {
            agendaServerSocket_.sendSTR("false");
        }
    }
}
void ServiceInterface::changeUserPassword(void) {
    std::string new_password;
    readInfomationFromUser(userName);       if (userName == "q") return;
    readInfomationFromUser(userPassword);   if (userPassword == "q") return;
    readInfomationFromUser(new_password);
    if (agendaServerSocket_.recvSTR() == "OK") {
        if (agendaService_->changeUserPassword(userName, userPassword, new_password)) {
            agendaServerSocket_.sendSTR("true");
        } else {
            agendaServerSocket_.sendSTR("false");
        }
    }
}
