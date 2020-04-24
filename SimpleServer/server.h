extern int start(int);
extern int acceptConnection();
extern int * getClients();
extern int closeConnection(int);
extern int send(int, char*);
extern int pingClient(int);
extern int broadcast(int*, int, char*);
extern char * receive(int);
