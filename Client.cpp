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
    //if( bind(getDescriptor(), (struct sockaddr *)&address, sizeof(address)) < 0 )
      //  perror("BIND ERROR");
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
        cout << "Peer IP:Port " << peer_address.sin_addr.s_addr << ":" << peer_address.sin_port << endl;
        cout << "Peer IP:Port " << inet_ntoa(peer_address.sin_addr) << ":" << ntohs(peer_address.sin_port) << endl;

        if( sendto(getDescriptor(), buffer, 1023, 0, (struct sockaddr*)&peer_address, sizeof(peer_address)) < 0 )
            perror("SEND STREAM TO PEER FAILED");
    }
    else {
        cout << "Control to server" << endl;
        if( sendto(getDescriptor(), buffer, 1023, 0, (struct sockaddr*)&server_address, sizeof(server_address)) < 0 )
            perror("SEND STREAM TO SERVER FAILED");
    }

}

string Client::ReadStream(struct sockaddr_in& cli, socklen_t& l){
    char buffer[1024];
    if( recvfrom(getDescriptor(), buffer, 1023, 0, (struct sockaddr*)&cli, &l) < 0)
        perror("READ STREAM FAILED");
    string data(buffer);
    return data;
}

void Client::setPeerAddress(string iport){

    size_t del = iport.find('/');

    string ip = iport.substr(0, del);
    string portnum = iport.substr(del + 1, iport.length() - del - 1);
    cout << "set peer address" << ip << ":" << portnum << endl;
    peer_address.sin_family = AF_INET;

    const char *cip = ip.c_str();
    struct hostent* peer = gethostbyname(cip);
    bcopy((char *)peer->h_addr,
          (char *)&peer_address.sin_addr.s_addr,
          peer->h_length);

    peer_address.sin_port = htons(stoi(portnum));

}


void Client::changePeerAddress(struct sockaddr_in& new_addr) {
    peer_address.sin_addr.s_addr = new_addr.sin_addr.s_addr;
    peer_address.sin_port = new_addr.sin_port;
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

string Client::getPeerPort() const{
    char port[128];
    sprintf(port, "%u", ntohs(peer_address.sin_port));
    string port_s(port);
    return port_s;
}

void Client::handleIncomingRequest(Request* new_request){

    char token = new_request->getCode();
    sockaddr_in cli;
    socklen_t l;
    string s;

    switch(token){
        case '1':
            cout << "Connection to server succeed My Id[" << new_request->getBody() << "]" << endl;
            break;

        case '2':
            cout << "Sender get peer IP" << endl;
            setPeerAddress(new_request->getBody());
            cout << "My peer address " << getPeerIP() << ":" << getPeerPort() << endl;
            SendStream("hello");
            SendStream("3" + getPeerIP() + "/" + getPeerPort(), false);
            break;

        case '3':
            cout << "receiver get peer IP" << endl;
            setPeerAddress(new_request->getBody());
            s = ReadStream(cli, l);
            cout << "received stream from peer " << s << endl;
            cout << "Real peer address " << inet_ntoa(cli.sin_addr) << ":" <<  ntohs(cli.sin_port) << endl;
            cout << "Real peer address " << cli.sin_addr.s_addr << ":" <<  cli.sin_port << endl;
            changePeerAddress(cli);
            cout << "My peer address after modification " << getPeerIP() << ":" << getPeerPort() << endl;
            break;

        case '4':
            cout << "server informs receiver about lost hello" << endl;
            cout << "My peer address " << getPeerIP() << ":" << getPeerPort() << endl;
            SendStream("hello");
            SendStream("4" + getPeerIP() + "/" + getPeerPort(), false);
            break;

        case '5':
            cout << "server informs sender that receiver sends a hello" << endl;
            SendStream("5" + getPeerIP() + "/" + getPeerPort(), false);
            SendStream("This is the sender");
            SendStream("This is the sender again");
            SendStream("This is the sender again and again");
            break;

        case '6':
            SendStream("This is the receiver");
            SendStream("This is the receiver again");
            SendStream("This is the receiver again and again");
            break;

        default:
            cout << new_request->getCode() << new_request->getBody() << endl;
            break;
    }
}
