//
// Created by oranngeoai on 1/12/18.
//

#include "Client.h"


Client::Client(int PORT_NUM, int SERV_PORT_NUM){
    this->PORT_NUM = PORT_NUM;
    this->SERV_PORT_NUM = SERV_PORT_NUM;
    setServer();
    CreateSocket();
}

void Client::setServer(){
    server_address.sin_family = AF_INET;

    struct hostent* server = gethostbyname("165.227.175.2");
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);

    server_address.sin_port = htons(SERV_PORT_NUM);
}

void Client::setAddress(){
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_NUM);
}

sockaddr* Client::getAddress() const{
    return (struct sockaddr *)&address;
}

void Client::CreateSocket(){

    setDescriptor(socket(AF_INET, SOCK_DGRAM, 0));
    setAddress();
    if( bind(getDescriptor(), (struct sockaddr *)&address, sizeof(address)) < 0 )
        perror("BIND ERROR");
}

void Client::setDescriptor(int descriptor){
    this->descriptor = descriptor;
    if(descriptor < 0) perror("FAILED TO CREATE SOCKET");
}

int Client::getDescriptor() const{
    return descriptor;
}

void Client::SendStream(string data, bool DATA){

    char buffer[1024];
    strcpy(buffer, data.c_str());

    if(DATA){
        cout << "Data sent to peer" << endl;
        if( sendto(getDescriptor(), buffer, 1023, 0, (struct sockaddr*)&peer_address, sizeof(peer_address)) < 0 )
            perror("SEND STREAM TO PEER FAILED");
    }
    else {
        cout << "Control to server" << endl;
        if( sendto(getDescriptor(), buffer, 1023, 0, (struct sockaddr*)&server_address, sizeof(server_address)) < 0 )
            perror("SEND STREAM TO SERVER FAILED");
    }

}

string Client::ReadStream(){
    char buffer[1024];
    if( recvfrom(getDescriptor(), buffer, 1023, 0, NULL, NULL) < 0)
        perror("READ STREAM FAILED");
    string data(buffer);
    return data;
}

void Client::setPeerAddress(string iport){

    size_t del = iport.find('/');

    string ip = iport.substr(0, del);
    string portnum = iport.substr(del + 1, iport.length() - del - 1);

    peer_address.sin_family = AF_INET;

    const char *cip = ip.c_str();
    struct hostent* peer = gethostbyname(cip);
    bcopy((char *)peer->h_addr,
          (char *)&peer_address.sin_addr.s_addr,
          peer->h_length);

    peer_address.sin_port = htons(stoi(portnum));

}


Request Client::AcceptRequest(){
    char buffer[1024];

    struct sockaddr_in cli_address;
    socklen_t l = sizeof(cli_address);

    if( recvfrom(getDescriptor(), buffer, 1023, 0, (struct sockaddr *)&cli_address, &l) < 0)
        perror("READ STREAM FAILED");

    string data(buffer);

    Request new_request(cli_address, data);

    return new_request;
}

string Client::getPeerIP() const{
    string ip(inet_ntoa(peer_address.sin_addr));
    return ip;
}


void Client::handleIncomingRequest(Request* new_request){

    char token = new_request->getCode();

    switch(token){
        case '1':
            cout << "Connection to server succeed My Id" << new_request->getBody() << endl;
            break;

        case '2':
            cout << "Sender get peer IP" << endl;
            setPeerAddress(new_request->getBody());
            SendStream("hello");
            SendStream("3" + getPeerIP(), false);
            break;

        case '3':
            cout << "receiver get peer IP" << endl;
            setPeerAddress(new_request->getBody());
            break;

        case '4':
            cout << "server informs receiver about lost hello" << endl;
            SendStream("hello");
            SendStream("4" + getPeerIP(), false);
            break;

        case '5':
            cout << "server informs sender that receiver sends a hello" << endl;
            break;
        default:
            cout << new_request->getCode() << new_request->getBody() << endl;
            break;
    }
}

