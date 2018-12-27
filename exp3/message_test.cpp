#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

struct message {
	long msg_type;
	char data[100];
};

void sender() {
	message msg;

	int msgid = msgget((key_t) 19562, 0644 | IPC_CREAT);
	if (msgid == -1) {
		std::cerr << "can not create/get a message queue." << std::endl;
		exit(-1);
	}
 
	while (true) {
		msg.msg_type = 555;
		strcpy(msg.data, "hello world!");
		if (msgsnd(msgid, (void*) &msg, sizeof(message), 0) == -1) {
			std::cerr << "can not send a message." << std::endl;
			exit(-1);
		}
		std::cout << "send a message<" << msg.msg_type << ">: " << msg.data << std::endl;
		sleep(1);
	}
}

void receiver() {
	message msg;

	int msgid = msgget((key_t) 19562, 0644 | IPC_CREAT);
	if (msgid == -1) {
		std::cerr << "can not create/get a message queue." << std::endl;
		exit(-1);
	}
 
	while (true) {
		if (msgrcv(msgid, (void*) &msg, sizeof(message), 555 /* 指定消息类型 */, 0) == -1) {
			std::cerr << "can not receive a message." << std::endl;
			exit(-1);
		}
		std::cout << "receive a message<" << msg.msg_type << ">: " << msg.data << std::endl;
	}
}

int main() {
	if (fork() == 0)
		receiver();
	else
		sender();
	return 0;
}