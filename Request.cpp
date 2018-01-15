//
// Created by oranngeoai on 1/12/18.
//

#include "Request.h"


Request::Request(SOCKADDR_IN address, string request){

    this->address = address;
    this->code = request[0];
    if(request.length() > 1)
        this->body = request.substr(1, request.length() - 1);
    else this->body = "";
    len = sizeof(address);
}

string Request::getBody() const{
    return body;
}

char Request::getCode() const{
    return code;
}

string Request::getIP() const{
    string ip(inet_ntoa(address.sin_addr));
    cout << "Request IP " << ip << endl;
    return ip;
}

string Request::getPort() const{
    char port[128];
    sprintf(port, "%u", ntohs(address.sin_port));
    string port_s(port);
    cout << "Request Port " << port_s << endl;
    return port_s;
}

SOCKADDR_IN Request::getAddress() const{
    return address;
}