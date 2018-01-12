//
// Created by oranngeoai on 1/12/18.
//

#ifndef UDP_RANDEVO_SERVER_USER_H
#define UDP_RANDEVO_SERVER_USER_H

#include "headers.h"

class User {
    int ID, index, descriptor;
    bool busy;
    SOCKADDR_IN address;
    socklen_t l;

public:

    User();

    void setBusy(bool);
    bool isBusy() const;

    void setID(int);
    int getID() const;

    void setIndx(int);
    int getIndx() const;

    void setDescriptor(int);
    int getDescriptor() const;

    void setAddress(SOCKADDR_IN);
    SOCKADDR_P getAddress() const;
    string getIP();
    string getPort();

    void writeToClient(string);
    string readFromClient();


    void connectToPeer(User*);
};


#endif //UDP_RANDEVO_SERVER_USER_H
