//
// Created by oranngeoai on 1/12/18.
//

#ifndef UDP_RANDEVO_SERVER_REQUEST_H
#define UDP_RANDEVO_SERVER_REQUEST_H

#include "headers.h"


class Request {
    SOCKADDR_IN address;
    socklen_t len;
    char code;
    string body;

public:
    Request(SOCKADDR_IN, string);

    string getBody() const;
    SOCKADDR_IN getAddress() const;
    char getCode() const;
    string getIP() const;
    string getPort() const;

};


#endif //UDP_RANDEVO_SERVER_REQUEST_H
