#include "Date.h"
#include <string>
#include <cstdio>
using std::string;

Date::Date() {
    year_ = 0;
    month_ = 0;
    day_ = 0;
    hour_ = 0;
    minute_ = 0;
}
Date::Date(int y, int m, int d, int h, int mi)
  : year_(y), month_(m), day_(d), hour_(h), minute_(mi) {}
int Date::getYear(void) const {
    return year_;
}
void Date::setYear(int year) {
    year_ = year;
}
int Date::getMonth(void) const {
    return month_;
}
void Date::setMonth(int month) {
    month_ = month;
}
int Date::getDay(void) const {
    return day_;
}
void Date::setDay(int day) {
    day_ = day;
}
int Date::getHour(void) const {
    return hour_;
}
void Date::setHour(int hour) {
    hour_ = hour;
}
int Date::getMinute(void) const {
    return minute_;
}
void Date::setMinute(int minute) {
    minute_ = minute;
}
bool Date::isValid(Date date) {
    int a[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int b[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (date.getYear() < 1000 || date.getYear() > 9999) return false;
    if (date.getMonth() <= 0 || date.getMonth() > 12) return false;
    if (date.getDay() <= 0) return false;
    if (date.getHour() < 0 || date.getHour() >= 24) return false;
    if (date.getMinute() < 0 || date.getMinute() >= 60) return false;
    // determine whether the day is valid
    if ((date.getYear()%4 == 0 && date.getYear()%100 != 0)
        || date.getYear()%400 == 0) {
        if (date.getDay() > b[date.getMonth()]) {
            return false;
        } else {
            return true;
        }
    } else {
        if (date.getDay() > a[date.getMonth()]) {
            return false;
        } else {
            return true;
        }
    }
}

Date Date::stringToDate(std::string date) {
    Date temp;
    int i = 0, d = -2;
    int year, month, day, hour, minute;
    year = month = day = hour = minute = 0;
    while (i < date.size()) {
        if (date[i] == '-' && d == -2) {
            d++; i++; continue;
        }
        if (date[i] == '-' && d == -1) {
            d++; i++; continue;
        }
        if (date[i] == '/' && d == 0) {
            d++; i++; continue;
        }
        if (date[i] == ':' && d == 1) {
            d++; i++; continue;
        }
        if (d == -2) year = year * 10 + (date[i] - '0');
        if (d == -1) month = month * 10 + (date[i] - '0');
        if (d == 0) day = day * 10 + (date[i] - '0');
        if (d == 1) hour = hour * 10 + (date[i] - '0');
        if (d == 2) minute = minute * 10 + (date[i] - '0');
        i++;
    }
    temp.setYear(year);
    temp.setMonth(month);
    temp.setDay(day);
    temp.setHour(hour);
    temp.setMinute(minute);
    return temp;
}
std::string Date::dateToString(Date date) {
    std::string dateString;
    if (!isValid(date)) {
        dateString = "0000-00-00/00:00";
        return dateString;
    }
    char str[17];
    int num;
    num = date.getYear();    // convert year
    for (int i = 3; i >= 0; --i) {
        str[i] = num % 10 + '0';
        num /= 10;
    }
    str[4] = '-';
    num = date.getMonth();    // convert month
    str[6] = num % 10 + '0';
    str[5] = (num/10) % 10 + '0';
    str[7] = '-';
    num = date.getDay();    // convert day
    str[9] = num % 10 + '0';
    str[8] = (num/10) % 10 + '0';
    str[10] = '/';
    num = date.getHour();    // convert hour
    str[12] = num % 10 + '0';
    str[11] = (num/10) % 10 + '0';
    str[13] = ':';
    num = date.getMinute();    // convert minute
    str[15] = num % 10 + '0';
    str[14] = (num/10) % 10 + '0';
    str[16] = '\0';
    dateString = str;
    return dateString;
}
Date& Date::operator=(const Date& date) {
    year_ = date.year_;
    month_ = date.month_;
    day_ = date.day_;
    hour_ = date.hour_;
    minute_ = date.minute_;
    return *this;
}
bool Date::operator==(const Date& date) const {
    if (year_ != date.year_)     return false;
    if (month_ != date.month_)   return false;
    if (day_ != date.day_)       return false;
    if (hour_ != date.hour_)     return false;
    if (minute_ != date.minute_) return false;
    return true;
}
bool Date::operator>(const Date& date) const {
    if (year_ > date.year_) {  // compare year
        return true;
    } else if (year_ == date.year_) {
        if (month_ > date.month_) {  // compare month
            return true;
        } else if (month_ == date.month_) {
            if (day_ > date.day_) {  // compare day
                return true;
            } else if (day_ == date.day_) {
                if (hour_ > date.hour_) {  // compare hour
                    return true;
                } else if (hour_ == date.hour_) {
                    if (minute_ > date.minute_) {  // compare minute
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}
bool Date::operator<(const Date& date) const {
    if (year_ < date.year_) {  // compare year
        return true;
    } else if (year_ == date.year_) {
        if (month_ < date.month_) {  // compare month
            return true;
        } else if (month_ == date.month_) {
            if (day_ < date.day_) {  // compare day
                return true;
            } else if (day_ == date.day_) {
                if (hour_ < date.hour_) {  // compare hour
                    return true;
                } else if (hour_ == date.hour_) {
                    if (minute_ < date.minute_) {  // compare minute
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}
bool Date::operator>=(const Date& date) const {
    if (year_ > date.year_) {  // compare year
        return true;
    } else if (year_ == date.year_) {
        if (month_ > date.month_) {  // compare month
            return true;
        } else if (month_ == date.month_) {
            if (day_ > date.day_) {  // compare day
                return true;
            } else if (day_ == date.day_) {
                if (hour_ > date.hour_) {  // compare hour
                    return true;
                } else if (hour_ == date.hour_) {
                    if (minute_ >= date.minute_) {  // compare minute
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}
bool Date::operator<=(const Date& date) const {
    if (year_ < date.year_) {  // compare year
        return true;
    } else if (year_ == date.year_) {
        if (month_ < date.month_) {  // compare month
            return true;
        } else if (month_ == date.month_) {
            if (day_ < date.day_) {  // compare day
                return true;
            } else if (day_ == date.day_) {
                if (hour_ < date.hour_) {  // compare hour
                    return true;
                } else if (hour_ == date.hour_) {
                    if (minute_ <= date.minute_) {  // compare minute
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

