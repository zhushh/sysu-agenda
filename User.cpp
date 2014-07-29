#include "User.h"
#include <string>
using std::string;

User::User() {
    name_ = "";
    password_ = "";
    email_ = "";
    phone_ = "";
}
User::User(string name, string passwd, string email, string phone)
  : name_(name), password_(passwd), email_(email), phone_(phone) {}
string User::getName() const {
    return name_;
}
void User::setName(string name) {
    name_ = name;
}
string User::getPassword() const {
    return password_;
}
void User::setPassword(string passwd) {
    password_ = passwd;
}
string User::getEmail() const {
    return email_;
}
void User::setEmail(string email) {
    email_ = email;
}
string User::getPhone() const {
    return phone_;
}
void User::setPhone(string phone) {
    phone_ = phone;
}
