OBJECTS_1=Date.o User.o Meeting.o AGsocket.o AgendaUI.o Client.o
OBJECTS_2=Date.o User.o Meeting.o AGsocket.o Storage.o AgendaService.o Service.o
OBJECTS=Date.o User.o Meeting.o AGsocket.o AgendaUI.o Client.o Storage.o AgendaService.o Service.o
EXEC_1=AgendaOfClient
EXEC_2=AGServer
CC=g++

all: EXEC_1 EXEC_2
EXEC_1:	$(OBJECTS_1)
	$(CC) -o $(EXEC_1) -std=c++0x $(OBJECTS_1)
Date.o: Date.h Date.cpp
	$(CC) -c -std=c++0x Date.h Date.cpp
User.o: User.h User.cpp
	$(CC) -c -std=c++0x User.h User.cpp
Meeting.o: Meeting.h Meeting.cpp
	$(CC) -c -std=c++0x Meeting.h Meeting.cpp
AGsocket.o:	AGsocket.h AGsocket.cpp
	$(CC) -c -std=c++0x AGsocket.h AGsocket.cpp
AgendaUI.o: AgendaUI.h AgendaUI.cpp
	$(CC) -c -std=c++0x AgendaUI.h AgendaUI.cpp
Client.o: Client.cpp
	$(CC) -c -std=c++0x Client.cpp
EXEC_2: $(OBJECTS_2)
	$(CC) -o $(EXEC_2) -std=c++0x $(OBJECTS_2)
Storage.o: Storage.h Storage.cpp
	$(CC) -c -std=c++0x Storage.h Storage.cpp
AgendaService.o: AgendaService.h AgendaService.cpp
	$(CC) -c -std=c++0x AgendaService.h AgendaService.cpp
Service.o: Service.cpp
	$(CC) -c -std=c++0x Service.cpp
clean:
	rm $(OBJECTS) $(EXEC_1) $(EXEC_2) *.h.gch
