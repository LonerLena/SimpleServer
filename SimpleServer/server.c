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
int start(int port) {
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
	clients[clientsIndex] = conn;
	while(clients[clientsIndex] != -1) {
		clientsIndex = (clientsIndex+1)%MAXCLIENTS;
	}
	return conn;
}

/**
 * Returns clients array.
 * @return int clients[].
 */
int* getClients() {
	return clients;
}

/**
 * Close an individual connection.
 * @param int conn, client.
 * @return int 0.
 */
int closeConnection(int conn) {
	close(conn);
	for(int i = 0; i < MAXCLIENTS; i++) {
		if(clients[i] == conn) {
			clients[i] = -1;
		}
	}
	return 0;
}

/**
 * Send message to client.
 * @param int conn, connection.
 * @param char msg, message.
 */
int send(int conn, char* msg) {
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
		if(clients[i] != -1) {
			sendMessage(clients[i], msg);
		}
	}
	return 0;
}

/**
 * Receive data from a single client.
 * @param int conn, connection.
 * @return char value.
 */
char* receive(int conn) {
	buffer = malloc(65535);
	int data = recv(conn, buffer, sizeof(buffer), 0);
	if(data < 0) {
		perror("recv");
		return("\0");
	} else if(data == 0) {
		printf("server: Client disconnected.");
		for(int i = 0; i < MAXCLIENTS; i++) {
			if(clients[i] == conn) {
				clients[i] == -1;
			}
		}
		return("\0");
	} else {
		buffer[data] = "\0";
		return(buffer);
	}

}