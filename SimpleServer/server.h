extern int serverAccept();
extern int serverBroadcast(char*);
extern int * serverClients();
extern int serverCloseAll();
extern int serverClose(int);
extern int serverSend(int, char*);
extern int serverStart(int);
extern int serverPingAll();
extern int serverPing(int);
extern char * serverReceive(int);
extern char * serverReceiveAny();