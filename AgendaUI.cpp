#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include <cstdlib>

#include "AgendaUI.h"
#include "Meeting.h"
#include "Date.h"
#include "AGsocket.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::setw;
using std::left;
using std::list;

void logOutPrint(void) {
    cout << "----------------------- Agenda -----------------------------" << endl
         << "Action:" << endl
         << "l/login        -   log in Agenda by name and password" << endl
         << "r/register     -   register an Agenda account" << endl
         << "q/quit         -   quit Agenda" << endl
         << "h/help         -   print help manual" << endl
         << "------------------------------------------------------------" << endl;
}
void logInPrint(void) {
    cout << "-------------------- Agenda Service -------------------------" << endl
         << "Action:" << endl
         << "o      -   log out Agenda" << endl
         << "dc     -   delete Agenda account" << endl
         << "lu     -   list all Agenda user" << endl
         << "cm     -   create a meeting" << endl
         << "la     -   list all meetings" << endl
         << "las    -   list all sponsor meetings" << endl
         << "lap    -   list all participate meetings" << endl
         << "qm     -   query meeting by title" << endl
         << "qt     -   query meeting by time interval" << endl
         << "dm     -   delete meeting by title" << endl
         << "da     -   delete all meetings" << endl
         << "cp     -   change user password" << endl
         << "cst    -   change meeting start time" << endl
         << "cet    -   change meeting end time" << endl
         << "ct     -   change meeting time" << endl
         << "lh     -   list all history that have been operated" << endl
         << "h/help -   print help manual" << endl
         << "------------------------------------------------------------" << endl;
}

// public
AgendaUI::AgendaUI() {}

void AgendaUI::OperationLoop(void) {
    string op = "";
    startAgenda();
    logOutPrint();
    updateSocket();
    while (true) {
        op = getOperation();
        if (!executeOperation(op)) {
            break;
        }
    }
    quitAgenda();
}

// private
// read a non-empty string
string AgendaUI::readString(string prompt) {
    string temp = "", result = "";
    while (result == "") {
        while (temp == "") {
            cout << prompt;
            getline(cin, temp);
        }
        size_t i = 0;
        while (i < temp.size() && temp[i] == ' ') {
            i++;
        }
        while (i < temp.size()) {
            result.push_back(temp[i]);
            ++i;
        }
        temp = "";
    }
    return result;
}

std::string AgendaUI::getOperation(void) {
    if (userName_.empty()) {
        return readString("Agenda : ~$ ");
    } else {
       return readString("Agenda@" + userName_ + " : # ");
    }
}
bool AgendaUI::executeOperation(std::string op) {
    if (!userName_.empty()) {
        agendaSocket_.sendSTR(op);
        if (agendaSocket_.recvSTR() == "OK") {
            if (op == "o") {
                userLogOut();
            } else if (op == "dc") {
                deleteUser();
            } else if (op == "lu") {
                listAllUsers();
            } else if (op == "cm") {
                createMeeting();
            } else if (op == "la") {
                listAllMeetings();
            } else if (op == "las") {
                listAllSponsorMeetings();
            } else if (op == "lap") {
                listAllParticipateMeetings();
            } else if (op == "qm") {
                queryMeetingByTitle();
            } else if (op == "qt") {
                queryMeetingByTimeInterval();
            } else if (op == "dm") {
                deleteMeetingByTitle();
            } else if (op == "da") {
                deleteAllMeetings();
            } else if (op == "h" || op == "help") {  // print log in help manual
                logInPrint();
            }
        }
    } else {
        if (op == "l" || op == "login") {
            agendaSocket_.sendSTR(op);
            if (agendaSocket_.recvSTR() == "OK") userLogIn();
        } else if (op == "r" || op == "register") {
            agendaSocket_.sendSTR(op);
            if (agendaSocket_.recvSTR() == "OK") userRegister();
        } else if (op == "q" || op == "quit") {
            agendaSocket_.sendSTR(op);
            if (agendaSocket_.recvSTR() == "OK") return false;
        } else if (op == "h" || op == "help") {  // print log out help manual
            logOutPrint();
        } else {  // prompt user input right operator
            cout << "Operator error! please input right operator ^_^" << endl;
        }
    }
    return true;
}

void AgendaUI::startAgenda(void) {
    agendaSocket_.start(false);     // start link to service
}
void AgendaUI::quitAgenda(void) {
    // userLogOut();
    agendaSocket_.quit();
}
bool AgendaUI::updateSocket(void) {
    return agendaSocket_.update();
}
void AgendaUI::userLogOut(void) {
    userName_.clear();
    userPassword_.clear();
    logOutPrint();
}
void AgendaUI::userLogIn(void) {
    cout << "[log in] " << endl;
    cout << "[user name] : ";
    getline(cin, userName_);        // cout << endl << userName_ << endl;
    cout << "[password] : ";
    getline(cin, userPassword_);    // cout << endl << userPassword_ << endl;

    if (!agendaSocket_.sendSTR(userName_)) cout << "send User name failed!" << endl;
    else cout << "[userName send] send user name succeed" << endl;

    if (agendaSocket_.recvSTR() == "OK") std::cout << "check and vertify user name" << endl;

    if (!agendaSocket_.sendSTR(userPassword_)) cout << "send user password failed" << endl;
    else cout << "[userPassword send] send user password succeed" << endl;

    if (agendaSocket_.recvSTR() == "OK") agendaSocket_.sendSTR("OK");

    if (agendaSocket_.recvSTR() == "true") {
        cout << "[log in] succeed!" << endl;
        logInPrint();  // print one time for every log in
    } else {
        cout << "[error] log in fail!" << endl;
        userName_.clear();
        userPassword_.clear();
    }
}
// better register interface
void AgendaUI::userRegister(void) {
    string prompt[] = {"[user name]: ", "[password]: ", "[email]: ", "[phone]: "};
    string info[4];
    cout << "[register (input 'q' to withdraw register)]" << endl;
    for (size_t i = 0; i < 4; ++i) {
        info[i] = readString(prompt[i]);
        if ("q" == info[i]) {
            cout << "quit to register!" << endl;
            return;
        }
    }

    for (size_t i = 0; i < 4; i++) {
        agendaSocket_.sendSTR(info[i]);
        if (agendaSocket_.recvSTR() == "OK")
            std::cout << "service get " << info[i] << endl;
    }
    agendaSocket_.sendSTR("OK");
    if ("true" == agendaSocket_.recvSTR()) {
        cout << "[register] succeed!" << endl;
    } else {
        cout << "[error] register fail!" << endl;
    }
}
void AgendaUI::deleteUser(void) {
    agendaSocket_.sendSTR(userName_);
    if (agendaSocket_.recvSTR() == "OK") cout << "service get userName" << endl;

    agendaSocket_.sendSTR(userPassword_);
    if (agendaSocket_.recvSTR() == "OK") agendaSocket_.sendSTR("OK");

    if ("true" == agendaSocket_.recvSTR()) {
        cout << "[delete agenda account] succeed!" << endl;
        userName_.clear();
        userPassword_.clear();
    } else {
        cout << "[delete agenda account] fail!" << endl;
    }
}
void AgendaUI::listAllUsers(void) {
    std::string result;
    cout << "[list all users]\n" << endl
         << std::left
         << std::setw(20) << "Name"
         << std::setw(20) << "Email"
         << std::setw(20) << "Phone" << endl;
    for (size_t count = 1; "OK" != (result = agendaSocket_.recvSTR()); count++) {
        cout << std::left << setw(20) << result;
        if (count % 3 == 0) cout << endl;
        agendaSocket_.sendSTR("OK");
    }
}
// create meeting and withdraw if you want
void AgendaUI::createMeeting(void) {
    char ch;
    string title, partor, start, end;
    string info[4];
    string prompt[] = {"[title]: ", "[participator]: ",
    "[start time(yyyy-mm-dd/hh:mm)]: ", "[end time(yyyy-mm-dd/hh:mm)]: "};
    cout << "[create meeting(input 'q' to withdraw)]" << endl;
    for (size_t i = 0; i < 4; ++i) {
        info[i] = readString(prompt[i]);
        while ("q" == info[i]) {
            cout << "Are you sure to give up to create meeting?(input y/n) ";
            while ((ch = cin.get())) {
                cin.ignore();
                if (ch == 'y' || ch == 'Y') {
                    cout << "[create meeting]: create meeting withdraw!" << endl;
                    return;
                } else if (ch == 'n' || ch == 'N') {
                    cout << "create meeting continue ..." << endl;
                    info[i] = readString(prompt[i]);
                    break;
                } else {
                    cout << "please select a choice.(y/n) " << endl;
                }
            }
        }
    }

    // send username, title, participator, start time and end time
    agendaSocket_.sendSTR(userName_);
    for (size_t i = 0; i < 4; i++) {
        if (agendaSocket_.recvSTR() == "OK")
            agendaSocket_.sendSTR(info[i]);
    }
    agendaSocket_.sendSTR("OK");
    if ("true" == agendaSocket_.recvSTR()) {
        cout << "[create meeting] succeed!" << endl;
    } else {
        cout << "[error] create meeting fail!" << endl;
    }
}





// get meeting by name
list<Meeting> AgendaUI::getMeetings() {
    std::list<Meeting> temp;
    std::string result = "";
    agendaSocket_.sendSTR("OK");
    while ("OK" != (result = agendaSocket_.recvSTR())) {
        Meeting data;
        data.setSponsor(result);
        agendaSocket_.sendSTR("OK");
        result = agendaSocket_.recvSTR();
        data.setParticipator(result);
        agendaSocket_.sendSTR("OK");
        result = agendaSocket_.recvSTR();
        data.setStartDate(Date::stringToDate(result));
        agendaSocket_.sendSTR("OK");
        result = agendaSocket_.recvSTR();
        data.setEndDate(Date::stringToDate(result));
        agendaSocket_.sendSTR("OK");

        data.setTitle(result);
        temp.push_back(data);
    }
    return temp;
}
void AgendaUI::listAllMeetings(void) {
    cout << endl << "[list all meetings]" << endl << endl;
    agendaSocket_.sendSTR(userName_);
    if (agendaSocket_.recvSTR() == "OK")
        printMeetings(getMeetings());
}
void AgendaUI::listAllSponsorMeetings(void) {
    cout << endl << "[list all sponsor meetings]" << endl << endl;
    agendaSocket_.sendSTR(userName_);
    if (agendaSocket_.recvSTR() == "OK")
        printMeetings(getMeetings());
}
void AgendaUI::listAllParticipateMeetings(void) {
    cout << endl << "[list all participate meetings]" << endl << endl;
    agendaSocket_.sendSTR(userName_);
    if (agendaSocket_.recvSTR() == "OK")
        printMeetings(getMeetings());
}
void AgendaUI::queryMeetingByTitle(void) {
    string title;
    list<Meeting> temp;
    cout << "[query meeting] [title]" << endl;
    title = readString("[query meeting]: ");
    agendaSocket_.sendSTR(userName_);
    if (agendaSocket_.recvSTR() == "OK")
        agendaSocket_.sendSTR(title);
    if (agendaSocket_.recvSTR() == "OK")
        temp = getMeetings();

    if (temp.size() > 0) {
        cout << std::left
             << std::setw(20) << "sponsor"
             << std::setw(20) << "participator"
             << std::setw(20) << "start time"
             << std::setw(20) << "end time" << endl;
        for (auto itr = temp.begin(); itr != temp.end(); ++itr) {
            cout << std::left
                 << std::setw(20) << itr->getSponsor()
                 << std::setw(20) << itr->getParticipator()
                 << std::setw(20) << Date::dateToString(itr->getStartDate())
                 << std::setw(20) << Date::dateToString(itr->getEndDate())
                 << endl;
        }
    } else {
        cout << "Title Meeting can not find!" << endl;
    }
}
void AgendaUI::queryMeetingByTimeInterval(void) {
    string st, et;
    cout << endl << "[query meetings]" << endl;
    st = readString("[start time(yyyy-mm-dd/hh:mm)]: ");
    et = readString("[end time(yyyy-mm-dd/hh:mm)]: ");
    agendaSocket_.sendSTR(userName_);
    if (agendaSocket_.recvSTR() == "OK")
        agendaSocket_.sendSTR(st);
    if (agendaSocket_.recvSTR() == "OK")
        agendaSocket_.sendSTR(et);
    if (agendaSocket_.recvSTR() == "OK")
        printMeetings(getMeetings());
}
void AgendaUI::deleteMeetingByTitle(void) {
    string title;
    cout << "[delete meeting] [title]" << endl;
    title = readString("[meeting title]: ");
    agendaSocket_.sendSTR(userName_);
    if (agendaSocket_.recvSTR() == "OK")
        agendaSocket_.sendSTR(title);
    if (agendaSocket_.recvSTR() == "OK")
        agendaSocket_.sendSTR("OK");
    if ("true" == agendaSocket_.recvSTR()) {
        cout << endl << "[delete meeting by title] succeed!" << endl;
    } else {
        cout << endl << "[error] delete meeting fail!" << endl;
    }
}
void AgendaUI::deleteAllMeetings(void) {
    agendaSocket_.sendSTR(userName_);
    if (agendaSocket_.recvSTR() == "OK")
        agendaSocket_.sendSTR("OK");
    if ("true" == agendaSocket_.recvSTR()) {
        cout << endl << "[delete meeting] succeed!" << endl;
    } else {
        cout << endl << "[error] delete meeting fail!" << endl;
    }
}
void AgendaUI::printMeetings(std::list<Meeting> meetings) {
    cout << std::left
         << std::setw(20) << "title"
         << std::setw(20) << "sponsor"
         << std::setw(20) << "participator"
         << std::setw(20) << "start time"
         << std::setw(20) << "end time" << endl;

    for (auto itr = meetings.begin(); itr != meetings.end(); ++itr) {
        cout << std::left
             << std::setw(20) << itr->getTitle()
             << std::setw(20) << itr->getSponsor()
             << std::setw(20) << itr->getParticipator()
             << std::setw(20) << Date::dateToString(itr->getStartDate())
             << std::setw(20) << Date::dateToString(itr->getEndDate())
             << endl;
    }
}
/*
// entension functions
// chang meeting start time
void AgendaUI::changeStartTime() {
    string title, newStartTime;
    cout << "[change meeting start time]" << endl;
    title = readString("[meeting title]: ");
    newStartTime = readString("[new meeting time]: ");
    if (agendaService_.changeStartTime(userName_, title, newStartTime)) {
        cout << "[change meeting start time] succeeded! ^_^" << endl;
    } else {
        cout << "[change meeting start time] failed! >_<" << endl;
    }
}
// change meeting end time
void AgendaUI::changeEndTime() {
    string title, newEndTime;
    cout << "[change meeting end time]" << endl;
    title = readString("[meeting title]: ");
    newEndTime = readString("[new meeting time]: ");
    if (agendaService_.changeEndTime(userName_, title, newEndTime)) {
        cout << "[change meeting end time] succeeded! ^_^" << endl;
    } else {
        cout << "[change meeting end time] failed! >_<" << endl;
    }
}
// change meeting time
void AgendaUI::changeMeetingTime() {
    string title, newStartTime, newEndTime;
    cout << "[change meeting time]" << endl;
    title = readString("[meeting title]: ");
    newStartTime = readString("[new meeting start time]: ");
    newEndTime = readString("[new meeting end time]: ");
    if (agendaService_.changeMeetingTime(userName_, title,
        newStartTime, newEndTime)) {
        cout << "[change meeting time] succeeded! ^_^" << endl;
    } else {
        cout << "[change meeting time] failed! >_<" << endl;
    }
}
// print history that user has operated
void AgendaUI::printHistory(void) {
    cout << "[print operator history]" << endl;
    if (!log_->printHistory()) {
        cout << "History file was empty." << endl;
    }
}
// change user password
void AgendaUI::changeUserPassword(void) {
    string oldPasswd, newPasswd_, newPasswd;
    cout << "[change user password]" << endl;
    cout << "[old password]: ";
    getline(cin, oldPasswd);
    cout << "[new password]: ";
    getline(cin, newPasswd);
    cout << "[confirm new password]: ";
    getline(cin, newPasswd_);
    if (oldPasswd != userPassword_) {
        cout << "[error]: Sorry, you input wrong old password! o_o" << endl;
    } else if (newPasswd_ != newPasswd) {
        cout << "[error]: Sorry, two new passwords are different! -_-" << endl;
    } else {
        if (agendaService_.changeUserPassword(userName_, oldPasswd, newPasswd)) {
            userPassword_ = newPasswd_;
            cout << "[change user password] succeeded! ^_^" << endl;
        } else {
            cout << "[change user password] failed! please try again >_<" << endl;
        }
    }
}

*/
