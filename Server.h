//
// Created by oranngeoai on 1/12/18.
//

#ifndef UDP_RANDEVO_SERVER_SERVER_H
#define UDP_RANDEVO_SERVER_SERVER_H

#include "User.h"
#include "Request.h"

class Server {
    int PORT_NUM, descriptor;
    SOCKADDR_IN address;

    static Server* s_instance;

    User* users[100];
    pthread_mutex_t users_mutex;
    int current_users;

    User* createUser(Request*);

    User* isExist(int);
    User* getByIPORT(string, string);

    Server();

public:


    static Server* instance();

    void setDescriptor(int);
    int getDescriptor() const;

    void setAddress();
    SOCKADDR_P getAddress() const;

    void createSocket(int);

    Request* AcceptRequest();
    void HandleRequest(Request*);

    void addUser(User*);
    void removeUser(User*);

};


#endif //UDP_RANDEVO_SERVER_SERVER_H
