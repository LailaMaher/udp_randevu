//
// Created by oranngeoai on 1/12/18.
//

#ifndef UDP_RANDEVO_SERVER_HEADERS_H
#define UDP_RANDEVO_SERVER_HEADERS_H

#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>

using namespace std;


#define SOCKADDR_IN struct sockaddr_in
#define SOCKADDR_P struct sockaddr*


#endif //UDP_RANDEVO_SERVER_HEADERS_H
