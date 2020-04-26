#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"

#define MAXCLIENTS 1024

int s, conn, clients[MAXCLIENTS];
int clientsIndex = 0;
char* buffer;
struct sockaddr_in server, client;

/**
 * Starts the TCP server.
 * @param int port, port for server to run on.
 * @return int 0 on success.
 */
int serverStart(int port) {
	s = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	// Set all values to -1 in clients.
	for(int i = 0; i < MAXCLIENTS; i++) {
		clients[i] = -1;
	}

	if (bind(s, (struct sockaddr*)&server, sizeof(server)) != 0) {
		perror("bind");
		return(1);
	}
	if(listen(s, 6) != 0) {
		perror("listen");
		return(2);
	}
	printf("[*] Started server.\n");
	return(0);
}

/**
 * Accepts an incoming connection.
 * @return index.
 */
int serverAccept() {
	conn = accept(s, (struct sockaddr*)&client, sizeof(client));
	if(conn < 0) {
		return -1;
	}
	clients[clientsIndex] = conn;
	int oldClientsIndex = clientsIndex;
	while(clients[clientsIndex] != -1) {
		clientsIndex = (clientsIndex+1)%MAXCLIENTS;
	}
	return oldClientsIndex;
}

/**
 * Returns clients array.
 * @return int clients[].
 */
int* serverClients() {
	return clients;
}

/**
 * Close an individual connection.
 * @param int index, client.
 * @return int 0.
 */
int serverClose(int index) {
	close(clients[index]);
	clients[index] = -1;
	return 0;
}

/**
 * Send message to client.
 * @param int index, connection.
 * @param char msg, message.
 */
int serverSend(int index, char* msg) {
	if(write(clients[index], msg, sizeof(msg)) != 0) {
		printf("[-] Client disconnected.\n");
		clients[index] == -1;
		return 1;
	}
	return 0;
}

//TODO: Check if connection is still alive.
int serverPing(int conn) {
	return 0;
}

/**
 * Send message to all clients.
 * @param int index, total amount of clients.
 * @param char msg, message.
 */
int serverBroadcast(char* msg) {
	for(int i = 0; i < MAXCLIENTS; i++) {
		if(clients[i] != -1) {
			serverSend(i, msg);
		}
	}
	return 0;
}

/**
 * Receive data from a single client.
 * @param int index, connection.
 * @return char value.
 */
char* serverReceive(int index) {
	buffer = malloc(65535);
	int data = recv(clients[index], buffer, sizeof(buffer), 0);
	if(data < 0) {
		perror("recv");
		return("\0");
	} else if(data == 0) {
		printf("[-] Client disconnected.\n");
		clients[index] == -1;
		return("\0");
	} else {
		buffer[data] = "\0";
		return(buffer);
	}

}