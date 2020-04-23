#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

int s, conn;
struct sockaddr_in server, client;

/**
 * Starts the TCP server.
 * @param int port, port for server to run on.
 * @return int 0 on success.
 */
int start(int port) {
	s = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) != 0) {
		perror("bind");
		return(1);
	}
	if(listen(s, 6) != 0) {
		perror("listen");
		return(2);
	}
	return(0);
}

/**
 * Accepts an incoming connection.
 * @return connection.
 */
int acceptConnection() {
	conn = accept(s, (struct sockaddr*)&client, sizeof(client));
	if(conn < 0) {
		return -1;
	}
	return conn;
}

int* getClients() {
	return 0;
}

//TODO: Ban an IP address.
int ban() {
	return 0;
}

//TODO: Kick a client.
int kick() {
	return 0;
}

/**
 * Send message to client.
 * @param int conn, connection.
 * @param char msg, message.
 */
int sendMessage(int conn, char* msg) {
	if(write(conn, msg, sizeof(msg)) != 0) {
		return 1;
	}
	return 0;
}

//TODO: Check if connection is still alive.
int pingClient(int conn) {
	return 0;
}

/**
 * Send message to all clients.
 * @param int index, total amount of clients.
 * @param char msg, message.
 */
int broadcast(int* clients, int index, char* msg) {
	for(int i = 0; i < index; i++) {
		sendMessage(clients[i], msg);
	}
	return 0;
}

//TODO: Receive messages.
char* receiveMessage() {
	return("");
}

