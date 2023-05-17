#ifndef _MY_GLOBAL
#define _MY_GLOBAL

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

extern int socket_file_descriptor, connection;
extern struct sockaddr_in serveraddress, client;
extern char messageCS[MESSAGE_LENGTH];

#endif