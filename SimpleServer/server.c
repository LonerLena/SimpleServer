#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 1337
#define BUFFER 4096

int main() {
	int s, conn, totalClients;
	struct sockaddr_in client, server;
	
	printf("Starting server...\n");
	
	s = socket(AF_INET, SOCK_STREAM, 0);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	if (bind(s, (struct sockaddr*)&server, sizeof(server)) != 0){
		perror("bind");
		exit(0);
	}

	if(listen(s, 6) != 0) {
		perror("listen");
		exit(0);
	}

	printf("Server started successfully!\n");
	totalClients = sizeof(client);
	while(1) {
		conn = accept(s, (struct sockaddr*)&client, &totalClients);
		if(conn < 0) {
			perror("accept");
		}
		printf("%d\n",conn);	
	}
}
