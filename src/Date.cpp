/*************************************************************************
 > File Name: Date.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Sat 18 Jul 2015 01:20:00 AM CST
 ************************************************************************/

#include "../headers/Date.h"
#include <stdio.h>

Date::Date() : year_(0), month_(0), day_(0), hour_(0), minute_(0) {}
Date::Date(int y, int m, int d, int h, int mi) :
	year_(y),
	month_(m),
	day_(d),
	hour_(h),
	minute_(mi) {}

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
	int m[] = {		// initialize month
		0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	if (date.getYear() < 1000 || date.getYear() > 9999) return false;
	if (date.getMonth() < 1 || date.getMonth() > 12) return false;
	if (date.getHour() < 0 || date.getHour() > 24) return false;
	if (date.getMinute() < 0 || date.getMinute() > 60) return false;
	if (date.getMonth() != 2) {		// month is Feb.
		if (date.getDay() < 0 || date.getDay() > m[date.getMonth()]) return false;
	} else {
		// leap year
		if (date.getYear() % 400 || (date.getYear() % 4 == 0 && date.getYear() % 100 != 0)) {
			if (date.getDay() < 0 || date.getDay() > 28) return false;
		} else {
			if (date.getDay() < 0 || date.getDay() > 29) return false;
		}
	}
	return true;
}

Date Date::stringToDate(std::string dateString) {
	Date tmp;
	int size = dateString.size();
	int num;
	int cur = -2;
	for (int i = 0; i < size; i++) {
        if (dateString[i] >= '0' && dateString[i] <= '9') {
            num = 0;
    		while (dateString[i] >= '0' && dateString[i] <= '9') {
    			num = num * 10 + (dateString[i++] - '0');
    		}
    		if (cur == -2) tmp.setYear(num);
    		if (cur == -1) tmp.setMonth(num);
    		if (cur == 0) tmp.setDay(num);
    		if (cur == 1) tmp.setHour(num);
    		if (cur == 2) tmp.setMinute(num);
    		++cur;
        }
	}
	return tmp;
}

std::string Date::dateToString(Date date) {
	std::string dtime;
	char tmp[10];
	sprintf(tmp, "%d", date.getYear());
	dtime += tmp;
    dtime += "-";
	sprintf(tmp, "%d", date.getMonth());
	if (date.getMonth() < 10) dtime.push_back('0');
	dtime += tmp;
    dtime += "-";
	sprintf(tmp, "%d", date.getDay());
	if (date.getDay() < 10) dtime.push_back('0');
	dtime += tmp;
    dtime += "/";
	sprintf(tmp, "%d", date.getHour());
	if (date.getHour() < 10) dtime.push_back('0');
	dtime += tmp;
    dtime += ":";
	sprintf(tmp, "%d", date.getMinute());
	if (date.getMinute() < 10) dtime.push_back('0');
	dtime += tmp;
	return dtime;
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
	if (year_ != date.year_) return false;
	if (month_ != date.month_) return false;
	if (day_ != date.day_) return false;
	if (hour_ != date.hour_) return false;
	if (minute_ != date.minute_) return false;
	return true;
}

bool Date::operator>(const Date& date) const {
	if (year_ < date.year_) {
		return false;
	} else if (year_ == date.year_) {
		if (month_ < date.month_) {
			return false;
		} else if (month_ == date.month_) {
			if (day_ < date.day_) {
				return false;
			} else if (day_ == date.day_) {
				if (hour_ < date.hour_) {
					return false;
				} else if (hour_ == date.hour_) {
					if (minute_ <= date.minute_) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

bool Date::operator<(const Date& date) const {
	return !(*this == date || *this > date);
}
bool Date::operator>=(const Date& date) const {
	return (*this == date || *this > date);
}
bool Date::operator<=(const Date& date) const {
	return (*this == date || *this < date);
}
