#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int port = 16352;

void error(const char* err) {
	std::cerr << err << std::endl;
}

void server() {
	int server_sockfd;
	int client_sockfd;
	sockaddr_in local_addr;
	sockaddr_in remote_addr;

	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = INADDR_ANY;
	local_addr.sin_port = htons(port);
	
	if ((server_sockfd = socket(PF_INET,SOCK_STREAM,0)) < 0) {  
		error("can not open socket.");
		exit(-1);
	}
 
	if (bind(server_sockfd, (sockaddr*) &local_addr, sizeof(sockaddr)) < 0) {
		error("can not bind IP address.");
		exit(-1);
	}
	
	if (listen(server_sockfd, 5) < 0) {
		error("can not listen on port.");
		exit(-1);
	}

	std::cout << "listen on port " << port << "..." << std::endl;
	
	unsigned int sin_size = sizeof(sockaddr_in);
	
	if ((client_sockfd = accept(server_sockfd, (sockaddr*) &remote_addr, &sin_size)) < 0) {
		error("can not accept the client socket.");
		exit(-1);
	}

	std::cout << "accept a client " << inet_ntoa(remote_addr.sin_addr) << std::endl;

	char buffer[100] = "hello world!";
	int len = write(client_sockfd, buffer, strlen(buffer));
	std::cout << "send " << len << " bytes to client: " << buffer << std::endl;
	
	while ((len = read(client_sockfd, buffer, 100)) > 0) {
		buffer[len] = 0;
		std::cout << "receive " << len << " bytes from client: " << buffer << std::endl;
		sleep(1);
		len = write(client_sockfd, buffer, strlen(buffer));
		std::cout << "send " << len << " bytes to client: " << buffer << std::endl;
	}

	close(client_sockfd);
	close(server_sockfd);
}

void client() {
	int client_sockfd;
	sockaddr_in remote_addr;
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote_addr.sin_port = htons(port);
	
	if ((client_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		error("can not open a client socket.");
		exit(-1);
	}
	
	if (connect(client_sockfd, (sockaddr*) &remote_addr, sizeof(sockaddr)) < 0) {
		error("can not connect the server socket.");
		exit(-1);
	}

	char buffer[100];
	int len = 0;
	while((len = read(client_sockfd, buffer, 100)) > 0) {
		buffer[len] = 0;
		write(client_sockfd, buffer, strlen(buffer));
	}

	close(client_sockfd);
}

int main() {
	if (fork() == 0)
		client();
	else
		server();
	return 0;
}