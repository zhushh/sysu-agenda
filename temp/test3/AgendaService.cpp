/*************************************************************************
 > File Name: AgendaService.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Mon 20 Jul 2015 12:44:25 AM CST
 ************************************************************************/

#include "AgendaService.h"

AgendaService::AgendaService() {
	storage_ = Storage::getInstance();
}

AgendaService::~AgendaService() {
	delete storage_;
}

bool AgendaService::userLogIn(std::string userName, std::string password) {
	std::list<User> tmp = storage_->queryUser([userName, password](const User& u) {
		return u.getName() == userName && u.getPassword() == password;
	});
	if (tmp.size() > 0) {
		return true;
	} else {
		return false;
	}
}

bool AgendaService::userRegister(std::string userName, std::string password,
                          std::string email, std::string phone) {
	std::list<User> tmp = storage_->queryUser([userName, password](const User &u) {
		return u.getName() == userName && u.getPassword() == password;
	});
	if (tmp.size() > 0) {
		return false;
	} else {
		storage_->createUser(User(userName, password, email, phone));
		return true;
	}
}

bool AgendaService::deleteUser(std::string userName, std::string password) {
	if (storage_->deleteUser([userName, password](const User &u) {
		return u.getName() == userName && u.getPassword() == password;
	}) > 0) {
		return true;
	} else {
		return false;
	}
}

// a user can only delete itself
std::list<User> AgendaService::listAllUsers(void) {
	return storage_->queryUser([](const User&u) {return true;});
}

bool AgendaService::createMeeting(std::string userName, std::string title,
                           std::string participator,
                           std::string startDate, std::string endDate) {
	Date stDate = Date::stringToDate(startDate);
	Date edDate = Date::stringToDate(endDate);
	// check the data is valid
	if ((storage_->queryUser([userName](const User &u) { return u.getName() == userName;})).size() == 0) {
		return false;
	}
	if ((storage_->queryUser([participator](const User &u) {return u.getName() == participator;})).size() == 0) {
		return false;
	}
	if (!Date::isValid(stDate)) {
		return false;
	}
	if (!Date::isValid(edDate)) {
		return false;
	}
	// check the logical is right
	if (stDate >= edDate) {
		return false;
	}
	if ((storage_->queryMeeting([&](const Meeting &m) {
		bool flag1 = (m.getSponsor() == userName || m.getSponsor() == participator);
		bool flag2 = (m.getParticipator() == userName || m.getParticipator() == participator);
		bool flag3 = (m.getStartDate() >= stDate && m.getStartDate() <= edDate);
		bool flag4 = (m.getEndDate() >= stDate && m.getEndDate() <= edDate);
		return ((flag1 || flag2) && (flag3 || flag4));
	})).size() > 0) {
		return false;
	}
	// succeed check
	storage_->createMeeting(Meeting(userName, participator, stDate, edDate, title));
	return true;
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName, std::string title) {
	return storage_->queryMeeting([userName, title](const Meeting &m) {
		return m.getTitle() == title && (m.getSponsor() == userName || m.getParticipator() == userName);
	});
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName, std::string startDate,
                                        std::string endDate) {
	Date stDate = Date::stringToDate(startDate);
	Date edDate = Date::stringToDate(endDate);
    if (stDate > edDate) {
        std::list<Meeting> tmp;     // logical error of query time
        return tmp;
    }
	return storage_->queryMeeting([&](const Meeting &m) {
		return (m.getSponsor() == userName || m.getParticipator() == userName)
		&& ((m.getStartDate() <= stDate && stDate <= m.getEndDate())
		|| (m.getStartDate() <= edDate && edDate <= m.getEndDate()));
	});
}

std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
	return storage_->queryMeeting([userName](const Meeting &m) {
		return m.getSponsor() == userName || m.getParticipator() == userName;
	});
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
	return storage_->queryMeeting([userName](const Meeting &m) {
		return m.getSponsor() == userName;
	});
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(std::string userName) {
	return storage_->queryMeeting([userName](const Meeting &m) {
		return m.getParticipator() == userName;
	});
}

bool AgendaService::deleteMeeting(std::string userName, std::string title) {
	if ((storage_->deleteMeeting([userName, title](const Meeting &m) {
		return (m.getSponsor() == userName || m.getParticipator() == userName) && m.getTitle() == title;
	})) > 0) {
		return true;
	} else {
		return false;
	}
}

bool AgendaService::deleteAllMeetings(std::string userName) {
	if ((storage_->deleteMeeting([](const Meeting &m) {return true;})) > 0) {
		return true;
	} else {
		return false;
	}
}

void AgendaService::startAgenda(void) {
	if (NULL == storage_) {
		storage_ = Storage::getInstance();
	}
}

void AgendaService::quitAgenda(void) {
	if (storage_ != NULL) {
		delete storage_;
	}
}
