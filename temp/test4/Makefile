OBJS=User.o Date.o Meeting.o Storage.o AgendaService.o AgendaUI.o main.o
CFLAGS=-Wall
EXEC=Agenda

$(EXEC):$(OBJS)
	g++ -std=c++0x $(CFLAGS) $(OBJS) -o $(EXEC)
main.o: main.cpp
	g++ -std=c++0x $(CFLAGS) main.cpp -c
AgendaUI.o: AgendaUI.h AgendaUI.cpp readPassword.h
	g++ -std=c++0x $(CFLAGS) AgendaUI.cpp readPassword.h -c
AgendaService.o: AgendaService.h AgendaService.cpp
	g++ -std=c++0x $(CFLAGS) AgendaService.cpp -c
Storage.o: Storage.h Storage.cpp
	g++ -std=c++0x $(CFLAGS) Storage.cpp -c
Meeting.o: Meeting.h Meeting.cpp
	g++ -std=c++0x $(CFLAGS) Meeting.cpp -c
Date.o: Date.h
	g++ -std=c++0x $(CFLAGS) Date.cpp -c
User.o: User.h
	g++ -std=c++0x $(CFLAGS) User.cpp -c
clean:
	rm $(OBJS) $(EXEC) user.csv meeting.csv *.h.gch
