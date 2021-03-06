#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "server.h"

#define MAXCLIENTS 1024
#define BUFFERSIZE 4096

int s, conn, clients[MAXCLIENTS];
int clientsIndex = 0;
char* buffer;
char bannedAddresses[MAXCLIENTS][32];
int bannedAddressIndex = 0;
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

	// Set all values to -1 in clients[].
	for(int i = 0; i < MAXCLIENTS; i++) {
		clients[i] = -1;
	}

	if (bind(s, (struct sockaddr*)&server, sizeof(server)) != 0) {
		perror("bind");
		return(1);
		exit(1);
	}
	if(listen(s, 8) != 0) {
		perror("listen");
		return(2);
		exit(2);
	}
	printf("[*] Started server.\n");
	return(0);
}

/**
 * Accepts an incoming connection.
 * @return index.
 */
int serverAccept() {
	int sizeOfClient = sizeof(client);
	conn = accept(s, (struct sockaddr*)&client, &sizeOfClient);
	if(conn < 0) {
		perror("accept");
		return -1;
	}
	clients[clientsIndex] = conn;
	int oldClientsIndex = clientsIndex;
	printf("[+] Client accepted with index: %d.\n",oldClientsIndex);
	if(serverBanCheckClient(oldClientsIndex) == 1) {
		printf("[!] A client from a banned IP address connected and will be removed.\n");
		serverClose(oldClientsIndex);
		return(-1);
	}
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
 * Returns IP address of peer.
 * @param int index, client.
 * @return char[]. IP address.
 */
char* serverGetClientAddress(int index) {
	struct sockaddr_in addr;
	socklen_t sizeOfAddr = sizeof(addr);
	if(getpeername(clients[index], (struct sockaddr*)&addr, &sizeOfAddr) == 0) {
		return(inet_ntoa(addr.sin_addr));
	} else {
		perror("getpeername");
		return("\0");
	}
}

int serverBanAddress(char* address) {
	strcpy(bannedAddresses[bannedAddressIndex], address);
	bannedAddressIndex = (bannedAddressIndex+1)%MAXCLIENTS;
	printf("[*] Banned address %s!\n",address);
	return(0);
}

int serverBanClient(int index) {
	char* address = serverGetClientAddress(index);
	if(address[0] != 0) {
		serverBanAddress(address);
	} else {
		return(-1);
	}
	return(0);
}

int serverBanCheckClient(int index) {
	for(int i = 0; i < MAXCLIENTS; i++) {
		if(strcmp(bannedAddresses[i], serverGetClientAddress(index)) == 0) {
			return 1;
		}
	}
	return 0;
}

int serverUnbanAddress(char* address) {
	for(int i = 0; i < MAXCLIENTS; i++) {
		if(strcmp(bannedAddresses[i],address) == 0) {
			strcpy(bannedAddresses[i], "\0");
			return 0;
		} 
	}
	return -1;
}

/**
 * Close an individual connection.
 * @param int index, client.
 * @return int 0.
 */
int serverClose(int index) {
	close(clients[index]);
	clients[index] = -1;
	printf("[-] Client %d disconnected.\n",index);
	return 0;
}

/**
 * Send message to client.
 * @param int index, connection.
 * @param char msg, message.
 */
int serverSend(int index, char* msg, int sizeOfMsg) {
	if(write(clients[index], msg, sizeOfMsg) == -1) {
		printf("[-] Client %d disconnected.\n",index);
		close(clients[index]);
		clients[index] = -1;
		return -1;
	}
	return 0;
}

/**
 * Check if connection is still alive. Closes connection with client if not alive.
 * @param int index, connection
 * @return int 0, success.
 */
int serverPing(int index) {
		if (recv(clients[index], buffer, BUFFERSIZE, MSG_PEEK | MSG_DONTWAIT) == 0) {
			serverClose(index);
		}
	return 0;
}

/**
 * Check if connection is still alive of all clients. Disconnects client if connection is not alive.
 * @return int 0, success.
 */
int serverPingAll() {
	for(int i = 0; i < MAXCLIENTS; i++) {
			serverPing(i);
	}
	return 0;
}

/**
 * Send message to all clients.
 * @param char msg, message.
 */
int serverBroadcast(char* msg, int sizeOfMsg) {
	for(int i = 0; i < MAXCLIENTS; i++) {
		if(clients[i] != -1) {
			serverSend(i, msg, sizeOfMsg);
		}
	}
	return 0;
}
/**
 * Close all connections.
 * @return int 0, success.
 */
int serverCloseAll() {
	for(int i = 0; i < MAXCLIENTS; i++) {
		if(clients[i] != -1) {
			serverClose(i);
		}
	}
	return(0);
}

/**
 * Receive data from a single client.
 * @param int index, connection.
 * @return char value.
 */
char* serverReceive(int index) {
	buffer = malloc(BUFFERSIZE);
	int data = recv(clients[index], buffer, BUFFERSIZE,0);
	if(data < 0) {
		perror("recv");
		return("\0");
	} else if(data == 0) {
		serverClose(index);
		return("\0");
	} else {
		buffer[data] = 0;
		buffer[data-1] = 0;
		return(buffer);
	}

}

/**
 * Receive message from any client.
 * @deprecated, not ready.
 */
char* serverReceiveAny() {
	buffer = malloc(BUFFERSIZE);
	for(int i = 0; i < MAXCLIENTS; i++) {
		if(clients[i] != -1) {
			int data = recv(clients[i], buffer, BUFFERSIZE, MSG_DONTWAIT);
			if(data < 0) {
			} else if(data == 0) {
				serverClose(i);
			} else {
				buffer[data] = 0;
				buffer[data-1] = 0;
				return(buffer);
			}
		}
	}
	return("");
}
