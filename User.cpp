//
// Created by oranngeoai on 1/12/18.
//


#include "User.h"

User::User(){
    setBusy(false);
}

void User::setBusy(bool busy){
    this->busy = busy;
}

bool User::isBusy() const{
    return busy;
}

void User::setIndx(int index){
    this->index = index;
}

int User::getIndx() const{
    return index;
}


void User::setID(int ID){
    this->ID = ID;
}

int User::getID() const{
    return ID;
}

void User::setAddress(SOCKADDR_IN address){
    this->address = address;
}

SOCKADDR_P User::getAddress() const{
    return (SOCKADDR_P ) &address;
}

string User::getIP() {
    string ip(inet_ntoa(address.sin_addr));
    return ip;
}

string User::getPort() {
    char port[128];
    sprintf(port, "%u", ntohs(address.sin_port));
    string port_s(port);
    return port_s;
}

void User::setDescriptor(int descriptor){
    this->descriptor = descriptor;
    if(descriptor < 0) perror("FAILED TO CREATE SOCKET");
}

int User::getDescriptor() const{
    return descriptor;
}

void User::connectToPeer(User* peer){

    if(!peer->isBusy()) {

        setBusy(true);
        peer->setBusy(true);

        cout << "Connect to peer function" << endl;
        cout << "2" + peer->getIP() + "/" + peer->getPort() << endl;
        cout << "3" + getIP() + "/" + getPort();

        writeToClient("2" + peer->getIP() + "/" + peer->getPort());
        peer->writeToClient("3" + getIP() + "/" + getPort());

    }

    else writeToClient("busy");
}

void User::writeToClient(string data){
    char buffer[1024];
    strcpy(buffer, data.c_str());

    if( sendto(getDescriptor(), buffer, 1023, 0, getAddress(), sizeof(address)) < 0 )
        perror("SEND STREAM FAILED");
}


string User::readFromClient(){
    char buffer[1024];
    l = sizeof(address);

    if( recvfrom(getDescriptor(), buffer, 1023, 0, getAddress(), &l) < 0)
        perror("READ STREAM FAILED");
    string data(buffer);
    return data;
}
