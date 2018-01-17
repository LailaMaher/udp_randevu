//
// Created by oranngeoai on 1/12/18.
//

#ifndef UDP_RANDEVO_SERVER_CLIENT_H
#define UDP_RANDEVO_SERVER_CLIENT_H

#include "Request.h"

class Client {

    int PORT_NUM, SERV_PORT_NUM, OTHER_PORT, OTHER_OTHER_PORT;
    int descriptor;
    SOCKADDR_IN address, peer_address, server_address, other_address, other_other_address;


public:

    Client(int, int, int);

    void setServer();
    void CreateSocket();
    void setAddress();
    sockaddr* getAddress() const;
    void setDescriptor(int);
    int getDescriptor() const;

    void SendStream(string, bool = true);
    string ReadStream(SOCKADDR_IN&, socklen_t&);

    void setPeerAddress(string);
    void changePeerAddress(SOCKADDR_IN&);
    string getPeerIP() const;
    string getPeerPort() const;

    Request AcceptRequest();
    void handleIncomingRequest(Request*);
};


#endif //UDP_RANDEVO_SERVER_CLIENT_H
