extern int serverStart(int);
extern int serverAccept();
extern int * serverClients();
extern int serverClose(int);
extern int serverSend(int, char*);
extern int serverPing(int);
extern int serverBroadcast(char*);
extern char * serverReceive(int);
