/*************************************************************************
 > File Name: Meeting.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Sat 18 Jul 2015 01:20:27 AM CST
 ************************************************************************/

#include "../headers/Meeting.h"
#include "../headers/Date.h"

Meeting::Meeting() {}
Meeting::Meeting(std::string sponsor, std::string participator,
        Date startTime, Date endTime, std::string title) :
	sponsor_(sponsor),
	participator_(participator),
	startDate_(startTime),
	endDate_(endTime),
	title_(title) {}

std::string Meeting::getSponsor(void) const {
	return sponsor_;
}
void Meeting::setSponsor(std::string sponsor) {
	sponsor_ = sponsor;
}
std::string Meeting::getParticipator(void) const {
	return participator_;
}
void Meeting::setParticipator(std::string participator) {
	participator_ = participator;
}
Date Meeting::getStartDate(void) const {
	return startDate_;
}
void Meeting::setStartDate(Date startTime) {
	startDate_ = startTime;
}
Date Meeting::getEndDate(void) const {
	return endDate_;
}
void Meeting::setEndDate(Date endTime) {
	endDate_ = endTime;
}
std::string Meeting::getTitle(void) const {
	return title_;
}
void Meeting::setTitle(std::string title) {
	title_ = title;
}
