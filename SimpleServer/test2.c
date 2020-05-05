#include <stdio.h>
#include <stdlib.h>
#include "server.h"

// Simple web server
// See rust book for a very simple web server: https://doc.rust-lang.org/stable/book/ch20-01-single-threaded.html
int main() {
    if(serverStart(8095) != 0) {
        exit(1);
    }
    
    int newclient = serverAccept();
    printf("%s\n",serverGetClientAddress(newclient));

	// Receive header
	// Header example:
	//
	// GET /page HTTP/1.1
    // Host: 127.0.0.1:8082
    // User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:76.0) Gecko/20100101 Firefox/76.0
    // Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
    // Accept-Language: en-US,en;q=0.5
    // Accept-Encoding: gzip, deflate
    // Connection: keep-alive
    // Upgrade-Insecure-Requests: 1

    char* receive = serverReceive(newclient);
	printf("%s\n",receive);

	// Response to client (html content)
	char msg[] = "HTTP/1.1 200 OK\r\n\r\n<h1>Response!</h1>\n\0";
	serverSend(newclient, msg, sizeof(msg));

    serverCloseAll();
}
