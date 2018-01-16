//
// Created by oranngeoai on 1/12/18.
//

#ifndef UDP_RANDEVO_SERVER_CLIENT_H
#define UDP_RANDEVO_SERVER_CLIENT_H

#include "Request.h"

class Client {

    int PORT_NUM, SERV_PORT_NUM;
    int descriptor;
    SOCKADDR_IN address, peer_address, server_address;


public:

    Client(int, int);

    void setServer();
    void CreateSocket();
    void setAddress();
    sockaddr* getAddress() const;
    void setDescriptor(int);
    int getDescriptor() const;

    void SendStream(string, bool = true);
    string ReadStream(struct sockaddr_in&, socklen_t&);

    void setPeerAddress(string);
    void changePeerAddress(struct sockaddr_in);
    string getPeerIP() const;
    string getPeerPort() const;

    Request AcceptRequest();
    void handleIncomingRequest(Request*);
};


#endif //UDP_RANDEVO_SERVER_CLIENT_H
