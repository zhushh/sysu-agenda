#ifndef _AGSOCKET_H_
#define _AGSOCKET_H_
#include <string> 
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLEN 10000                  //the max length of MSG buf
#define DEFAULT_PORT 8000             //Port
#define DEFAULT_IP "0.0.0.0"          //service address

class AGsocket {
	private:
		bool isService;              //is true when this code in service
		int socket_fd;               
		int connect_fd;              //(socketfd = connectfd) when this code in client
		struct sockaddr_in servaddr;
		
	public:
		AGsocket();
		~AGsocket();

		void start(bool);            //initialize member,such as socketfd
		void quit();                 //close socketfd and connectfd
		
		bool update();               //decide connect to which IP Adress
		                             //it will false if no service/client connect
		                             //it will true if something connect
		                             //if this code in a service, update will change the connectfd
		                             //you can loop wait until update is true
		
		
		std::string recvSTR();       //recieve MSG from connectfd
		                             
		bool sendSTR(std::string s); //send MSG to connectfd
};

#endif
