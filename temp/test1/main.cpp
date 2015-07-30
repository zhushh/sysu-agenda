/*************************************************************************
 > File Name: main.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Sun 19 Jul 2015 11:23:38 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

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
    Storage *ins = Storage::getInstance();
    ins->createUser(u1);
    ins->createUser(u2);

    cout << "Current user size = " << (ins->queryUser([](const User &u) {
        return true;
    })).size() << endl;

    ins->createMeeting(m1);
    ins->createMeeting(m2);

    cout << "Current meeting size = " << (ins->queryMeeting([](const Meeting &m) {
        return true;
    })).size() << endl;
    delete ins;

    ins = Storage::getInstance();
    cout << "Test query user:" << endl << "\t";
    (ins->queryUser([](const User &u) {
        return u.getName() == "zz";
    })).size() == 1 ? cout << "True" << endl : cout << "False" << endl;

    cout << "Test update user:" << endl << "\t";
    (ins->updateUser([](const User &u){
        return u.getName() == "zz";
    }, [](User &u){
        u.setName("bb");
    })) == 1 ? cout << "True" << endl : cout << "False" << endl;

    cout << "Test query meeting:" << endl << "\t";
    (ins->queryMeeting([](const Meeting &m) {
        return m.getTitle() == "Test1";
    })).size() == 1 ? cout << "True" << endl : cout << "False" << endl;

    cout << "Test update meeting:" << endl << "\t";
    (ins->updateMeeting([](const Meeting &m) {
        return m.getTitle() == "Test1";
    }, [](Meeting &m) {
        m.setTitle("Test3");
    })) == 1 ? cout << "True" << endl : cout << "False" << endl;
    delete ins;
    return 0;
}
