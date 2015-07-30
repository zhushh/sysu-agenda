#include "../headers/AGsocket.h"

AGsocket::AGsocket(){}
AGsocket::~AGsocket(){}
		
void AGsocket::start(bool ser) {
	isService = ser;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	connect_fd = socket_fd;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(DEFAULT_PORT);
	inet_pton(AF_INET, DEFAULT_IP, &servaddr.sin_addr);
	if (ser) {
		bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
		listen(socket_fd, 10);
	}
}
void AGsocket::quit() {
	if (isService) {
		close(socket_fd);
	} else {
		close(connect_fd);
		close(socket_fd);
	}
}

bool AGsocket::update() {
	if (isService) {
		// if (connect_fd != socket_fd) return true;
		if (connect_fd != socket_fd) close(connect_fd);
		return ((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) != -1);
	}
	else
		return (connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) >= 0);
}

std::string AGsocket::recvSTR() {
	char buf[MAXLEN];
	int recv_len = recv(connect_fd, buf, MAXLEN, 0);
	std::stringstream sin;
	for (int i = 0; i < recv_len; i++) sin << buf[i];
	return sin.str(); 
}
bool AGsocket::sendSTR(std::string s) {
	char buf[MAXLEN];
	int send_len = s.length();
	for (int i = 0; i< send_len; i++) buf[i] = s[i];
	buf[send_len] = '\0';
	if (send(connect_fd, buf, send_len, 0) == send_len) {
		return true;
	} else {
		return false;
	}
}
