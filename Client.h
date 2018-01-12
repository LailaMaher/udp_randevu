//
// Created by oranngeoai on 1/12/18.
//

#ifndef UDP_RANDEVO_SERVER_CLIENT_H
#define UDP_RANDEVO_SERVER_CLIENT_H

#include "Request.h"

class Client {

    int PORT_NUM;
    int descriptor;
    SOCKADDR_IN address, peer_address, server_address;


public:

    Client(int);

    void setServer();
    void CreateSocket();
    void setAddress();
    sockaddr* getAddress() const;
    void setDescriptor(int);
    int getDescriptor() const;

    void SendStream(string, bool = true);
    string ReadStream();

    void setPeerAddress(string);
    string getPeerIP() const;

    Request AcceptRequest();
    void handleIncomingRequest(Request*);
};


#endif //UDP_RANDEVO_SERVER_CLIENT_H
