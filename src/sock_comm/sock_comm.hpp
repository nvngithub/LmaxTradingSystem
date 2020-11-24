#ifndef SOCK_COMM_H
#define SOCK_COMM_H

#include <string>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

class SocketCommunicator
{
    private:
        SSL *ssl;
        int socketId;
        

    public:
        bool IsSocketConnected;
        int Init(std::string hostname, int port);
        std::string RecvPacket();
        int SendPacket(const char *buffer);    
    
};

#endif