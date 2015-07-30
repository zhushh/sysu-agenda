/*************************************************************************
 > File Name: AgendaUI.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Tue 21 Jul 2015 07:56:12 AM CST
 ************************************************************************/

#include "../headers/AgendaUI.h"
#include "../headers/readPassword.h"

#include <iostream>
#include <iomanip>
using std::cout;
using std::cin;
using std::endl;

AgendaUI::AgendaUI() {
    #ifdef SOCKET_PROGRAM
    clientService_.start(false);
    #endif
}

void AgendaUI::OperationLoop(void) {
    std::string op = "";
    #ifdef SOCKET_PROGRAM
    while (!clientService_.update());
    #endif
    quitAgenda();
    while (true) {
        op = getOperation();
        if (!executeOperation(op)) {
            break;
        }
    }
}

// task functions
void AgendaUI::quitAgenda(void) {
    cout << "----------------------- Agenda -----------------------------" << endl
         << "Action:" << endl
         << "l/login        -   log in Agenda by name and password" << endl
         << "r/register     -   register an Agenda account" << endl
         << "q/quit         -   quit Agenda" << endl
         << "h/help         -   print help manual" << endl
         << "------------------------------------------------------------" << endl;
}

void AgendaUI::startAgenda(void) {
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
         << "h/help -   print help manual" << endl
         << "------------------------------------------------------------" << endl;
}

// read a non-empty string
std::string readString(std::string prompt) {
    std::string temp = "", result = "";
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
    #ifdef SOCKET_PROGRAM
    while (!clientService_.sendSTR(op)) {}
    std::string answer;
    if ((answer = clientService_.recvSTR()) != "GET") {
        std::cout << "Receive from server: " << answer << std::endl;
        return false;
    }
    #endif
    if (!userName_.empty()) {
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
            startAgenda();
        }
    } else {
        if (op == "l" || op == "login") {
            userLogIn();
        } else if (op == "r" || op == "register") {
            userRegister();
        } else if (op == "q" || op == "quit") {
            return false;
        } else if (op == "h" || op == "help") {  // print log out help manual
            quitAgenda();
        } else {  // prompt user input right operator
            cout << "Operator error! please input right operator ^_^" << endl;
        }
    }
    return true;
}

void AgendaUI::userLogIn(void) {
    cout << "[log in] " << endl;
    cout << "[user name] : ";
    getline(cin, userName_);        // cout << endl << userName_ << endl;
    cout << "[password] : ";
    getline(cin, userPassword_);    // cout << endl << userPassword_ << endl;
//    readPassword(userPassword_, "[password] : ");

#ifdef SOCKET_PROGRAM
    if (!clientService_.sendSTR(userName_)) {
        std::cout << "[login in] fail!" << "Reason: "
                  << "User Name send fail!" << std::endl;
        return;
    }
    if (clientService_.recvSTR() == "Get") {
        if (!clientService_.sendSTR(userPassword_)) {
            std::cout << "[login in] fail!" << "Reason: "
                      << "User Password send fail!" << std::endl;
            return;
        }
    } else {
        std::cout << "[login in] fail!" << "Reason: "
                  << "cann't get response from server" << std::endl;
        return;
    }
    std::string answer;
    if ((answer = clientService_.recvSTR()) == "true")
#else
    if (agendaService_.userLogIn(userName_, userPassword_))
#endif
    {
        cout << "[log in] succeed!" << endl;
        startAgenda();  // print one time for every log in
    } else {
        cout << "[error] log in fail!" << endl;
#ifdef SOCKET_PROGRAM
        cout << "Receive message from server: " << answer << endl;
#endif
        userName_.clear();
        userPassword_.clear();
    }
}

// better register interface
void AgendaUI::userRegister(void) {
    std::string prompt[] = {"[user name]: ", "[password]: ", "[email]: ", "[phone]: "};
    std::string info[4];
    cout << "[register (input '!' to withdraw register)]" << endl;
    for (size_t i = 0; i < 4; ++i) {
        info[i] = readString(prompt[i]);
        if ("!" == info[i]) {
            cout << "quit to register!" << endl;
            return;
        }
    }

#ifdef SOCKET_PROGRAM
    if (!clientService_.sendSTR(info[0])) {
        std::cout << "Register Fail! Reason: "
                  << "Server do get user name" << std::endl;
        return;
    }
    if (clientService_.recvSTR() == "Get") {
        clientService_.sendSTR(info[1]);
        if (clientService_.recvSTR() == "Get") {
            clientService_.sendSTR(info[2]);
            if (clientService_.recvSTR() == "Get") {
                clientService_.sendSTR(info[3]);
            }
        }
    }
    if (clientService_.recvSTR() == "true")
#else
    if (agendaService_.userRegister(info[0], info[1], info[2], info[3])) 
#endif
    {
        cout << "[register] succeed!" << endl;
    } else {
        cout << "[error] register fail!" << endl;
    }
}

void AgendaUI::userLogOut(void) {
	userName_.clear();
	userPassword_.clear();
	quitAgenda();
}

void AgendaUI::deleteUser(void) {
#ifdef SOCKET_PROGRAM
    if (clientService_.recvSTR() == "true")
#else
    if (agendaService_.deleteUser(userName_, userPassword_)) 
#endif
    {
        cout << "[delete agenda account] succeed!" << endl;
        userName_.clear();
        userPassword_.clear();
    } else {
        cout << "[delete agenda account] fail!" << endl;
    }
}

void AgendaUI::listAllUsers(void) {
    std::string result;
    std::list<User> ulist = agendaService_.listAllUsers();
    cout << "[list all users] total " << ulist.size() << endl << endl
         << std::left
         << std::setw(20) << "Name"
         << std::setw(20) << "Email"
         << std::setw(20) << "Phone" << endl;

    for (auto ptr = ulist.begin(); ptr != ulist.end(); ptr++) {
    	//cout << std::left << setw(20) << result;
    	cout << std::left << std::setw(20) << ptr->getName();
    	cout << std::left << std::setw(20) << ptr->getEmail();
    	cout << std::left << std::setw(20) << ptr->getPhone() << endl;
    }
    cout << endl;
}

// create meeting and withdraw if you want
void AgendaUI::createMeeting(void) {
    char ch;
    std::string title, partor, start, end;
    std::string info[4];
    std::string prompt[] = {"[title]: ", "[participator]: ",
    "[start time(yyyy-mm-dd/hh:mm)]: ", "[end time(yyyy-mm-dd/hh:mm)]: "};
    cout << "[create meeting(input '!' to withdraw)]" << endl;
    for (size_t i = 0; i < 4; ++i) {
        info[i] = readString(prompt[i]);
        while ("!" == info[i]) {
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

void AgendaUI::printMeetings(const std::list<Meeting> &meetings) {
    cout << "Total " << meetings.size() << endl;
    cout << std::left
         << std::setw(20) << "Title"
         << std::setw(20) << "Sponsor"
         << std::setw(20) << "Participator"
         << std::setw(20) << "Start time"
         << std::setw(20) << "End time" << endl;

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

void AgendaUI::listAllMeetings(void) {
    cout << endl << "[list all meetings]" << endl << endl;
    printMeetings(agendaService_.listAllMeetings(userName_));
}

void AgendaUI::listAllSponsorMeetings(void) {
    cout << endl << "[list All sponsor meetings]" << endl << endl;
    printMeetings(agendaService_.listAllSponsorMeetings(userName_));
}

void AgendaUI::listAllParticipateMeetings(void) {
    cout << endl << "[list All participate meetings]" << endl << endl;
    printMeetings(agendaService_.listAllParticipateMeetings(userName_));
}

void AgendaUI::queryMeetingByTitle(void) {
    std::string title;
    std::list<Meeting> mlist;
    cout << "[query meeting] [title]" << endl;
    title = readString("[query meeting]: ");
    mlist = agendaService_.meetingQuery(userName_, title);

    if (mlist.size() > 0) {
    	printMeetings(mlist);
    } else {
        cout << "Title Meeting can not find!" << endl;
    }
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    std::string st, et;
    cout << endl << "[query meetings]" << endl;
    st = readString("[start time(yyyy-mm-dd/hh:mm)]: ");    // cout << st << endl;
    et = readString("[end time(yyyy-mm-dd/hh:mm)]: ");      // cout << et << endl;
    printMeetings(agendaService_.meetingQuery(userName_, st, et));
}

void AgendaUI::deleteMeetingByTitle(void) {
    std::string title;
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

//std::string AgendaUI::getOperation();
//bool AgendaUI::executeOperation(std::string op);
//void AgendaUI::userLogIn(void);
//void AgendaUI::userRegister(void);
//void AgendaUI::quitAgenda(void);
//void AgendaUI::userLogOut(void);
//void AgendaUI::deleteUser(void);
//void AgendaUI::listAllUsers(void);
//void AgendaUI::createMeeting(void);
//void AgendaUI::listAllMeetings(void);
//void AgendaUI::listAllSponsorMeetings(void);
//void AgendaUI::listAllParticipateMeetings(void);
//void AgendaUI::queryMeetingByTitle(void);
//void AgendaUI::queryMeetingByTimeInterval(void);
//void AgendaUI::deleteMeetingByTitle(void);
//void AgendaUI::deleteAllMeetings(void);
//void AgendaUI::printMeetings(std::list<Meeting> meetings);
