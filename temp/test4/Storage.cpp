/*************************************************************************
 > File Name: Storage.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Sat 18 Jul 2015 01:20:36 AM CST
*************************************************************************/

#include "Storage.h"

#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;

#include <string>
#include <cstring>
#include <cstdio>

#define DEFAULT_PATH "./"
#define MAXCHAR 4089

Storage* Storage::instance_ = NULL;
Storage::Storage() {
    readFromFile(DEFAULT_PATH);
}
// File IO
bool Storage::readFromFile(const char *fpath) {
    std::string fuser(fpath);
    fuser += "user.csv";
    std::string fmeeting(fpath);
    fmeeting += "meeting.csv";
    char line[MAXCHAR];
    FILE *fp;
    if ((fp = fopen(fuser.c_str(), "r")) == NULL) {
        return false;
    }
    // start read user data from file
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        User u;
        int d = 1;
        int len = strlen(line);
        if (len < 12) {     // invalid User data
            continue;
        }
        for (int i = 0; i < len; i++) {
            if (line[i] == '\"') {
                std::string stemp = "";
                i += 1;
                while (i < len && line[i] != '\"') {
                    stemp.push_back(line[i++]);
                }
                if (d == 1) u.setName(stemp);
                if (d == 2) u.setPassword(stemp);
                if (d == 3) u.setEmail(stemp);
                if (d == 4) u.setPhone(stemp);
                d++;
            }
        }
        userList_.push_back(u);
    }
    fclose(fp);

    if ((fp = fopen(fmeeting.c_str(), "r")) == NULL) {
        return false;
    }
    // start read meeting data from file
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        Meeting m;
        int d = 1;
        int len = strlen(line);
        if (len < 14) {     // invalid Meeting data
            continue;
        }
        for (int i = 0; i < len; i++) {
            if (line[i] == '\"') {
                std::string stemp = "";
                i += 1;
                while (i < len && line[i] != '\"') {
                    stemp.push_back(line[i++]);
                }
                if (d == 1) m.setSponsor(stemp);
                if (d == 2) m.setParticipator(stemp);
                if (d == 3) m.setStartDate(Date::stringToDate(stemp));
                if (d == 4) m.setEndDate(Date::stringToDate(stemp));
                if (d == 5) m.setTitle(stemp);
                d++;
            }
        }
        meetingList_.push_back(m);
    }
    fclose(fp);
    return true;
}

bool Storage::writeToFile(const char *fpath) {
    std::string fuser(fpath);
    fuser += "user.csv";
    std::string fmeeting(fpath);
    fmeeting += "meeting.csv";
    std::ofstream fout;
    fout.open(fuser.c_str(), std::ofstream::out);
    if (!fout.is_open()) {
        return false;
    }
    fout << "userName\tuserPassword\tuserEmail\tuserPhone\n";
    for (auto ptr = userList_.begin(); ptr != userList_.end(); ptr++) {
        fout << "\"" << ptr->getName() << "\",";
        fout << "\"" << ptr->getPassword() << "\",";
        fout << "\"" << ptr->getEmail() << "\",";
        fout << "\"" << ptr->getPhone() << "\"\n";
    }
    fout.close();

    fout.open(fmeeting.c_str(), std::ofstream::out);
    if (!fout.is_open()) {
        return false;
    }
    fout << "Sponsor\tParticipator\tStartTime\tEndTime\tTitle\n";
    for (auto ptr = meetingList_.begin(); ptr != meetingList_.end(); ptr++) {
        fout << "\"" << ptr->getSponsor() << "\",";
        fout << "\"" << ptr->getParticipator() << "\",";
        fout << "\"" << Date::dateToString(ptr->getStartDate()) << "\",";
        fout << "\"" << Date::dateToString(ptr->getEndDate()) << "\",";
        fout << "\"" << ptr->getTitle() << "\"\n";
    }
    fout.close();
    return true;
}

// singleton
Storage* Storage::getInstance(void) {
    if (NULL == instance_) {
        instance_ = new Storage();
    }
    return instance_;
}
Storage::~Storage() {
    writeToFile(DEFAULT_PATH);
    instance_ = NULL;
}
// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions
void Storage::createUser(const User& u) {
    userList_.push_back(u);
}
std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) {
    std::list<User> tmp;
    for (auto ptr = userList_.begin(); ptr != userList_.end(); ptr++) {
        if (filter(*ptr)) {
            tmp.push_back(*ptr);
        }
    }
    return tmp;
}
// return found users
int Storage::updateUser(std::function<bool(const User&)> filter,
            std::function<void(User&)> switcher) {
    int count = 0;
    for (auto ptr = userList_.begin(); ptr != userList_.end(); ptr++) {
        if (filter(*ptr)) {
            switcher(*ptr);
            count++;
        }
    }
    return count;
}
// return the number of updated users
int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int count = 0;
    for (auto ptr = userList_.begin(); ptr != userList_.end();) {
        if (filter(*ptr)) {
            ptr = userList_.erase(ptr);
            count++;
        } else {
            ptr++;
        }
    }
    return count;
}
// return the number of deleted users
void Storage::createMeeting(const Meeting& m) {
    meetingList_.push_back(m);
}
std::list<Meeting> Storage::queryMeeting(
                           std::function<bool(const Meeting&)> filter) {
    std::list<Meeting> tmp;
    for (auto ptr = meetingList_.begin(); ptr != meetingList_.end(); ptr++) {
        if (filter(*ptr)) {
            tmp.push_back(*ptr);
        }
    }
    return tmp;
}
// return found meetings
int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
            std::function<void(Meeting&)> switcher) {
    int count = 0;
    for (auto ptr = meetingList_.begin(); ptr != meetingList_.end(); ptr++) {
        if (filter(*ptr)) {
            switcher(*ptr);
            count++;
        }
    }
    return count;
}
// return the number of updated meetings
int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    int count = 0;
    for (auto ptr = meetingList_.begin(); ptr != meetingList_.end();) {
        if (filter(*ptr)) {
            ptr = meetingList_.erase(ptr);
            count++;
        } else {
            ptr++;
        }
    }
    return count;
}
// return the number of deleted meetings
// File IO
bool Storage::sync(void) {
    if (writeToFile(DEFAULT_PATH)) {
        return true;
    } else {
        return false;
    }
}
