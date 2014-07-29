#include "Storage.h"
#include <list>
#include <string>
#include <functional>

#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>

#include "User.h"
#include "Meeting.h"
using std::list;
using std::string;
using std::function;
using std::ifstream;
using std::ofstream;
using std::cerr;
using std::ios;
using std::cout;
using std::endl;

Storage* Storage::instance_ = NULL;

// read from file
bool Storage::readFromFile(const char* fpath) {
    ifstream source;
    source.open(fpath, std::fstream::in);   // remember using fstream::in
    if (!source) {    // check that the open succeeded
        return false;
    }
    userList_.clear();
    meetingList_.clear();
    size_t u_total = 0, m_total = 0, start = 0, end = 0;
    string str;
    string user[4], meet[5];
    string u[] = {"name:\"", "password:\"", "email:\"", "phone:\""};
    string m[] = {"sponsor:\"", "participator:\"", "sdate:\"", "edate:\"", "title:\""};
    // read User from source file
    getline(source, str);
    start = str.find("total:") + 7;   // caution! do not calculate wrong index
    end = str.length() - 2;   // caution! do not calculate wrong index
    for (size_t i = start; i < end; ++i) {
        u_total = u_total * 10 + (str[i] - '0');
    }
    for (size_t i = 0; i < u_total; ++i) {
        getline(source, str);
        for (size_t j = 0; j < 4; ++j) {
            user[j] = "";
            for (size_t k = str.find(u[j]) + u[j].size(); str[k] != '\"'; ++k) {
                user[j].push_back(str[k]);
            }
        }
        User temp(user[0], user[1], user[2], user[3]);
        userList_.push_back(temp);
    }
    // read meeting from source file
    getline(source, str);
    start = str.find("total:") + 7;   // caution! do not calculate wrong index
    end = str.length() - 2;   // caution! do not calculate wrong index
    for (size_t i = start; i < end; ++i) {
        m_total = m_total * 10 + (str[i] - '0');
    }
    for (size_t i = 0; i < m_total; ++i) {
        getline(source, str);
        for (size_t j = 0; j < 5; ++j) {
            meet[j] = "";
            for (size_t k = str.find(m[j]) + m[j].size(); str[k] != '\"'; ++k) {
                meet[j].push_back(str[k]);
            }
        }
        Date d1 = Date::stringToDate(meet[2]), d2 = Date::stringToDate(meet[3]);
        Meeting temp(meet[0], meet[1], d1, d2, meet[4]);
        meetingList_.push_back(temp);
    }
    source.close();    // remember close file
    return true;
}
// write infomation to file
bool Storage::writeToFile(const char* fpath) {
    ofstream dest;
    dest.open(fpath, std::fstream::out);
    if (!dest) {    // check that the open succeeded
        cerr << "File can't open!" << endl;
        return false;
    }
    size_t count = userList_.size();  // user number
    string summary, container;
    // write User's list to file
    summary = "{collection:\"User\",total:\"";
    container.push_back(count % 10 + '0');
    for (count /= 10; count > 0; count /= 10) {
        container.push_back(count % 10 + '0');
    }
    for (int i = container.size() - 1; i >= 0; --i) {
        summary.push_back(container[i]);
    }
    summary += "\"}\n";
    dest << summary;
    for (auto p = userList_.begin(); p != userList_.end(); p++) {
        container = "{name:\"" + p->getName() + "\",";
        container += "password:\"" + p->getPassword() + "\",";
        container += "email:\"" + p->getEmail() + "\",";
        container += "phone:\"" + p->getPhone() + "\"}\n";
        dest << container;
    }
    container = "";    // clear the container
    // write meeting's list to file
    count = meetingList_.size();  // meeting number
    summary = "{collection:\"Meeting\",total:\"";
    container.push_back(count % 10 + '0');
    for (count /= 10; count > 0; count /= 10) {
        container.push_back(count % 10 + '0');
    }
    for (int i = container.size() - 1; i >= 0; --i) {
        summary.push_back(container[i]);
    }
    summary += "\"}\n";
    dest << summary;
    for (auto p = meetingList_.begin(); p != meetingList_.end(); p++) {
        container = "{sponsor:\"" + p->getSponsor() + "\",";
        container += "participator:\"" + p->getParticipator() + "\",";
        container += "sdate:\"" + Date::dateToString(p->getStartDate()) + "\",";
        container += "edate:\"" + Date::dateToString(p->getEndDate()) + "\",";
        container += "title:\"" + p->getTitle() + "\"}\n";
        dest << container;
    }
    dest.close();    // remember close file
    return true;
}
// singleton (static)
Storage* Storage::getInstance(void) {
    if (instance_ == NULL) {
        instance_ = new Storage();
    }
    return instance_;
}
Storage::Storage() {
    readFromFile("agenda.data");
}
Storage::~Storage() {
    sync();
    instance_ = NULL;
}
// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions
void Storage::createUser(const User& newUser) {
    userList_.push_back(newUser);
}
std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) {
    list<User> temp;
    list<User>::iterator p = userList_.begin();
    for (; p != userList_.end(); ++p) {
        if (filter(*p)) {
            temp.push_back(*p);
        }
    }
    return temp;
}
// return found users
int Storage::updateUser(std::function<bool(const User&)> filter,
    std::function<void(User&)> switcher) {
    int count = 0;
    list<User>::iterator p = userList_.begin();
    for (; p != userList_.end(); ++p) {
        if (filter(*p)) {
            switcher(*p);
            count++;
        }
    }
    return count;
}
// return the number of updated users
int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int count = 0;
    list<User>::iterator p = userList_.begin();
    for (; p != userList_.end();) {
        if (filter(*p)) {
            p = userList_.erase(p);    // easy error
            count++;
        } else {
            ++p;
        }
    }
    return count;
}
// return the number of deleted users
void Storage::createMeeting(const Meeting& newMeeting) {
    meetingList_.push_back(newMeeting);
}
std::list<Meeting> Storage::queryMeeting(
                   std::function<bool(const Meeting&)> filter) {
    list<Meeting> temp;
    list<Meeting>::iterator p = meetingList_.begin();
    for (; p != meetingList_.end(); ++p) {
        if (filter(*p)) {
            temp.push_back(*p);
        }
    }
    return temp;
}
// return found meetings
int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
    std::function<void(Meeting&)> switcher) {
    int count = 0;
    list<Meeting>::iterator p = meetingList_.begin();
    for (; p != meetingList_.end(); ++p) {
        if (filter(*p)) {
            switcher(*p);
            count++;
        }
    }
    return count;
}
// return the number of updated meetings
int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    int count = 0;
    list<Meeting>::iterator p = meetingList_.begin();
    for (; p != meetingList_.end();) {
        if (filter(*p)) {
            p = meetingList_.erase(p);    // easy error
            count++;
        } else {
            ++p;
        }
    }
    return count;
}
// return the number of deleted meetings
// File IO
bool Storage::sync(void) {
    if (writeToFile("agenda.data")) {
        return true;
    } else {
        return false;
    }
}
