#ifndef AGENDAUI_H
#define AGENDAUI_H

#define SOCKET_PROGRAM
#ifdef SOCKET_PROGRAM
#include "AGsocket.h"
#endif

#include "AgendaService.h"
#include <iostream>
#include <string>

class AgendaUI {
 public:
  AgendaUI();
  void OperationLoop(void);

 private:
  // task functions
  void startAgenda(void);
  std::string getOperation();
  bool executeOperation(std::string op);
  void userLogIn(void);
  void userRegister(void);
  void quitAgenda(void);
  void userLogOut(void);
  void deleteUser(void);
  void listAllUsers(void);
  void createMeeting(void);
  void listAllMeetings(void);
  void listAllSponsorMeetings(void);
  void listAllParticipateMeetings(void);
  void queryMeetingByTitle(void);
  void queryMeetingByTimeInterval(void);
  void deleteMeetingByTitle(void);
  void deleteAllMeetings(void);
  void printMeetings(const std::list<Meeting> &meetings);
  // dates
  std::string userName_;
  std::string userPassword_;
//#ifndef SOCKET_PROGRAM
  AgendaService agendaService_;
//#else
  AGsocket clientService_;
//#endif
};

#endif
