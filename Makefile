OBJECTS=Date.o User.o Meeting.o Storage.o AgendaService.o AgendaUI.o Agenda.o
EXEC=Agenda
CC=g++

EXEC: Date.o User.o Meeting.o Storage.o AgendaService.o AgendaUI.o Agenda.o
	$(CC) -o $(EXEC) -std=c++0x Date.o User.o Meeting.o Storage.o AgendaService.o AgendaUI.o Agenda.o
Date.o: Date.h Date.cpp
	$(CC) -c -std=c++0x Date.h Date.cpp
User.o: User.h User.cpp
	$(CC) -c -std=c++0x User.h User.cpp
Meeting.o: Meeting.h Meeting.cpp
	$(CC) -c -std=c++0x Meeting.h Meeting.cpp
Storage.o: Storage.h Storage.cpp
	$(CC) -c -std=c++0x Storage.h Storage.cpp
AgendaUI.o: AgendaUI.h AgendaUI.cpp
	$(CC) -c -std=c++0x AgendaUI.h AgendaUI.cpp
AgendaService.o: AgendaService.h AgendaService.cpp
	$(CC) -c -std=c++0x AgendaService.h AgendaService.cpp
Agenda.o: Agenda.cpp
	$(CC) -c -std=c++0x Agenda.cpp
clean:
	rm $(OBJECTS) $(EXEC) *.h.gch
