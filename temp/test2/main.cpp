/*************************************************************************
 > File Name: main.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Sun 19 Jul 2015 11:23:38 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include "AgendaService.h"
#include "User.h"
#include "Date.h"
#include "Meeting.h"
#include "Storage.h"

int main() {
    User u1("zz", "zz", "zz", "zz");
    User u2("aa", "aa", "aa", "aa");
    Date d1(2013, 12, 10, 12, 10);
    Date d2(2013, 12, 11, 12, 10);
    Date d3(Date::stringToDate("2014-10-11/08:00"));
    Date d4(Date::stringToDate("2014-10-11/12:10"));
    Meeting m1("aa", "zz", d1, d2, "Test1");
    Meeting m2("aa", "zz", d3, d4, "Test2");

    AgendaService ags;
    ags.userRegister("zz", "zz", "zz", "zz");
    ags.userRegister("aa", "aa", "aa", "aa");

    list<User> ulist = ags.listAllUsers();
    cout << "Current user size = " << ulist.size() << endl;

    ags.userRegister("aa", "aa", "aa", "aa");
    cout << "Current user size = " << ags.listAllUsers().size() << endl;

    if (ags.userLogIn("aa", "aa")) {
        cout << "User aa login succeed!" << endl;
    } else {
        cout << "User aa login fail!" << endl;
    }

    ags.createMeeting("aa", "test title 1", "zz", "2013-10-11/12:00", "2013-10-11/13:00");
    ags.createMeeting("zz", "test title 2", "aa", "2013-11-12/13:10", "2013-11-12/15:20");

    list<Meeting> mlist = ags.listAllMeetings("aa");
    cout << "Current meeting size = " << mlist.size() << endl;  // answer is 2

    cout << "aa sponsor's meeting size = " << ags.listAllSponsorMeetings("aa").size() << endl;  // answer is 1
    cout << "a sponsor's meeting size = " << ags.listAllSponsorMeetings("a").size() << endl;    // answer is 0

    cout << "query meeting by title 1: " << ags.meetingQuery("aa", "test title 1").size() << endl;  // answer is 1
    cout << "query meeting by title 1: " << ags.meetingQuery("xaa", "test title 1").size() << endl; // answer is 0

    return 0;
}
