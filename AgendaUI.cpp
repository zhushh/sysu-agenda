#include "AgendaUI.h"
#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include <cstdlib>
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
         << "q/o    -   log out Agenda" << endl
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
         << "h/help -   print help manual" << endl
         << "------------------------------------------------------------" << endl;
}
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
/*
// get a no empty string
string getString() {
    string result = "";
    char c = cin.get();
    while (c == ' ' || c == '\t' || c == '\n') {
        c = cin.get();
    }
    result.push_back(c);
    c = cin.get();
    while (c != '\n') {
        result.push_back(c);
        c = cin.get();
    }
    return result;
}
*/

// public
AgendaUI::AgendaUI() {
    agendaService_.startAgenda();
}
void AgendaUI::OperationLoop(void) {
    string op = "";
    startAgenda();
    logOutPrint();
    while (true) {
        /*
        if (userName_.empty() || op == "r") {
            cout << endl << "Agenda : ~$ ";
        } else {
            cout << endl << "Agenda@" << userName_ <<" : # ";
        }
        */
        op = getOperation();
        if (!executeOperation(op)) {
            break;
        }
    }
}
// private
// task functions
void AgendaUI::startAgenda(void) {
    agendaService_.startAgenda();
}
// better getOperation
std::string AgendaUI::getOperation() {
    if (userName_.empty()) {
        return readString("Agenda : ~$ ");
    } else {
       return readString("Agenda@" + userName_ + " : # ");
    }
}
/*
std::string AgendaUI::getOperation() {
    string op;
    cin >> op;
    return op;
}
*/
bool AgendaUI::executeOperation(std::string op) {
    if (!userName_.empty()) {
        if (op == "o" || op == "q") {
system("clear");
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
        } else if (op == "cst") {  // change meeting start time
            changeStartTime();
        } else if (op == "cet") {  // change meeting end time
            changeEndTime();
        } else if (op == "ct") {  // change meeting time
            changeMeetingTime();
        } else if (op == "cp") {
            changeUserPassword();
        } else {
            cout << "Operator error! please input right operator ^_^" << endl;
        }
    } else {
        if (op == "l" || op == "login") {
system("clear");
            userLogIn();
        } else if (op == "r" || op == "register") {
            userRegister();
        } else if (op == "q" || op == "quit") {
            quitAgenda();
            return false;
        } else if (op == "h" || op == "help") {  // print log out help manual
            logOutPrint();
        } else {  // prompt user input right operator
            cout << "Operator error! please input right operator ^_^" << endl;
        }
    }
    return true;
}
void AgendaUI::userLogIn(void) {
    cout << "[log in (input 'q' to withdraw register)] " << endl;
    cout << "[user name] : ";
    getline(cin, userName_);
    if ("q" == userName_) {
        userName_.clear();
        userPassword_.clear();
        cout << "quit to login!" << endl;
        return;
    }
    cout << "[password] : ";
    getline(cin, userPassword_);
    if ("q" == userPassword_) {
        userName_.clear();
        userPassword_.clear();
        cout << "quit to login!" << endl;
        return;
    }
    // cout << "[log in] ";
    // cin >> userName_ >> userPassword_;
    bool flag = agendaService_.userLogIn(userName_, userPassword_);
    if (flag) {
system("clear");
        cout << "[log in] succeed!" << endl;
        logInPrint();  // print one time for every log in
    } else {
        cout << "[error] log in fail!" << endl;
        if (agendaService_.queryUser(userName_)) {
            cout << "Error password." << endl;
        } else {
            cout << "User name does not exist -_-" << endl;
        }
        userName_.clear();
        userPassword_.clear();
    }
}
/*
void AgendaUI::userRegister(void) {
    string name, passwd, email, phone;
    cout << "[register] [user name] [password] [email] [phone]" << endl;
    cout << "[register] ";
    cin >> name >> passwd >> email >> phone;
    if (agendaService_.userRegister(name, passwd, email, phone)) {
        cout << "[register] succeed!" << endl;
    } else {
        cout << "[error] register fail!" << endl;
    }
}
*/
// better register interface
void AgendaUI::userRegister(void) {
    // string name, passwd, email, phone;
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
    if (agendaService_.userRegister(info[0], info[1], info[2], info[3])) {
        cout << "[register] succeed!" << endl;
    } else {
        cout << "[error] register fail!" << endl;
    }
}
void AgendaUI::quitAgenda(void) {
    agendaService_.quitAgenda();
}
void AgendaUI::userLogOut(void) {
    agendaService_.quitAgenda();
    userName_.clear();
    userPassword_.clear();
    logOutPrint();
}
void AgendaUI::deleteUser(void) {
    cout << "Are you sure to delete your account?(y/n) ";
    string answer_;
    getline(cin, answer_);
    bool flag = (answer_ == "y" || answer_ == "Y") ? true : false;
    if (flag && agendaService_.deleteUser(userName_, userPassword_)) {
        cout << "[delete agenda account] succeed!" << endl;
        userName_.clear();
        userPassword_.clear();
    }
}
void AgendaUI::listAllUsers(void) {
    list<User> temp = agendaService_.listAllUsers();
    cout << "[list all users]\n" << endl
         << std::left
         << std::setw(20) << "name"
         << std::setw(20) << "email"
         << std::setw(20) << "phone" << endl;
    for (auto itr = temp.begin(); itr != temp.end(); ++itr) {
        cout << std::left
             << std::setw(20) << itr->getName()
             << std::setw(20) << itr->getEmail()
             << std::setw(20) << itr->getPhone()
             << endl;
    }
    cout << endl;
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
    if (agendaService_.createMeeting(userName_, info[0], info[1], info[2], info[3])) {
        cout << "[create meeting] succeed!" << endl;
    } else {
        cout << "[error] create meeting fail!" << endl;
    }
}
void AgendaUI::listAllMeetings(void) {
    cout << endl << "[list all meetings]" << endl << endl;
    printMeetings(agendaService_.listAllMeetings(userName_));
    cout << endl;
}
void AgendaUI::listAllSponsorMeetings(void) {
    cout << endl << "[list all sponsor meetings]" << endl << endl;
    printMeetings(agendaService_.listAllSponsorMeetings(userName_));
    cout << endl;
}
void AgendaUI::listAllParticipateMeetings(void) {
    cout << endl << "[list all participate meetings]" << endl << endl;
    printMeetings(agendaService_.listAllParticipateMeetings(userName_));
    cout << endl;
}
void AgendaUI::queryMeetingByTitle(void) {
    string title;
    cout << "[query meeting] [title]" << endl;
    title = readString("[query meeting]: ");
    list<Meeting> temp = agendaService_.meetingQuery(userName_, title);
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
    cout << endl;
}
void AgendaUI::queryMeetingByTimeInterval(void) {
    string st, et;
    cout << endl << "[query meetings]" << endl;
    st = readString("[start time(yyyy-mm-dd/hh:mm)]: ");
    et = readString("[end time(yyyy-mm-dd/hh:mm)]: ");
    printMeetings(agendaService_.meetingQuery(userName_, st, et));
    cout << endl;
}
void AgendaUI::deleteMeetingByTitle(void) {
    string title;
    cout << "[delete meeting] [title]" << endl;
    title = readString("[meeting title]: ");
    if (agendaService_.deleteMeeting(userName_, title)) {
        cout << endl << "[delete meeting by title] succeed!" << endl;
    } else {
        cout << endl << "[error] delete meeting fail!" << endl;
    }
}
void AgendaUI::deleteAllMeetings(void) {
    if (agendaService_.deleteAllMeetings(userName_)) {
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
// entension functions
// chang meeting start time
void AgendaUI::changeStartTime() {
    string title, newStartTime;
    cout << "[change meeting start time]" << endl;
    title = readString("[meeting title]: ");
    if ((agendaService_.meetingQuery(userName_, title)).empty()) {
        cout << "[error]: do not exist such a meeting! o_o\n" << endl;
        return;
    }
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
    if ((agendaService_.meetingQuery(userName_, title)).empty()) {
        cout << "[error]: do not exist such a meeting! o_o\n" << endl;
        return;
    }
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
    if ((agendaService_.meetingQuery(userName_, title)).empty()) {
        cout << "[error]: do not exist such a meeting! o_o\n" << endl;
        return;
    }
    newStartTime = readString("[new meeting start time]: ");
    newEndTime = readString("[new meeting end time]: ");
    if (agendaService_.changeMeetingTime(userName_, title,
        newStartTime, newEndTime)) {
        cout << "[change meeting time] succeeded! ^_^" << endl;
    } else {
        cout << "[change meeting time] failed! >_<" << endl;
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

