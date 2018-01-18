//
// Created by oranngeoai on 1/12/18.
//

#include "Client.h"


Client::Client(int SERV_PORT_NUM, int OTHER_PORT, int OTHER_OTHER_PORT){
    //this->PORT_NUM = PORT_NUM;
    this->SERV_PORT_NUM = SERV_PORT_NUM;
    this->OTHER_PORT = OTHER_PORT;
    this->OTHER_OTHER_PORT = OTHER_OTHER_PORT;
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
//
//    /*
//     * Second server
//     */
//
//
//    other_address.sin_family = AF_INET;
//
//    struct hostent* other = gethostbyname("165.227.175.2");
//    bcopy((char *)other->h_addr,
//          (char *)&other_address.sin_addr.s_addr,
//          other->h_length);
//
//    other_address.sin_port = htons(OTHER_PORT);
//
//
//
//    /*
//     * third server
//     */
//
//
//    other_other_address.sin_family = AF_INET;
//
//    struct hostent* other_other = gethostbyname("165.227.175.2");
//    bcopy((char *)other_other->h_addr,
//          (char *)&other_other_address.sin_addr.s_addr,
//          other_other->h_length);
//
//    other_other_address.sin_port = htons(OTHER_OTHER_PORT);

}

void Client::setAddress(){
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(OTHER_PORT);
}

sockaddr* Client::getAddress() const{
    return (struct sockaddr *)&address;
}

void Client::CreateSocket(){

    setDescriptor(socket(AF_INET, SOCK_DGRAM, 0));

    p2pdesc = socket(AF_INET, SOCK_DGRAM, 0);
    if(p2pdesc < 0) perror("FAILED TO CREATE P2P SOCKET");

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
        if( sendto(p2pdesc, buffer, 1023, 0, (struct sockaddr*)&peer_address, sizeof(peer_address)) < 0 )
        {
            string err = "READ STREAM FAILED PORT NUM = " + to_string(ntohs(peer_address.sin_port)) + " ";
            perror(err.c_str());
        }
    }
    else {

//        cout << "\t\t Data sent to servers" << endl;
        cout << "\t\t -- Data --to--> Server" << endl;
        sendto(descriptor, buffer, 1023, 0, (struct sockaddr*)&server_address, sizeof(server_address));
//        cout << "\t\t -- Data --to--> Server[2]" << endl;
//        sendto(descriptor, buffer, 1023, 0, (struct sockaddr*)&other_address, sizeof(other_address));
//        cout << "\t\t -- Data --to--> Server[3]" << endl;
//        sendto(descriptor, buffer, 1023, 0, (struct sockaddr*)&other_other_address, sizeof(other_other_address));

    }

}

Request Client::ReadStream(){

    char buffer[1024];

    struct sockaddr_in cli_address;
    socklen_t l = sizeof(cli_address);

    if( recvfrom(p2pdesc, buffer, 1023, 0, (struct sockaddr *)&cli_address, &l) < 0){
        string err = "READ STREAM FAILED PORT NUM = " + ntohs(cli_address.sin_port);
        perror(err.c_str());
    }

    string data(buffer);

    Request new_request(cli_address, data);

    return new_request;

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


void Client::changePeerAddress(SOCKADDR_IN& new_addr) {
    memset(&peer_address, 0, sizeof(peer_address));
    peer_address = new_addr;
}


Request Client::AcceptRequest(){
    char buffer[1024];

    struct sockaddr_in cli_address;
    socklen_t l = sizeof(cli_address);

    if( recvfrom(getDescriptor(), buffer, 1023, 0, (struct sockaddr *)&cli_address, &l) < 0){
        string err = "READ STREAM FAILED PORT NUM = " + ntohs(cli_address.sin_port);
        perror(err.c_str());
    }

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
    SOCKADDR_IN cli;
    socklen_t l;
    string s;
    char buffer[1024] = "this is a buffer :D";

    cout << "\t ---- Handle new request ---- " << endl;
    cout << "\t\t\t -- This is a request from " << new_request->getIP() << ":" << new_request->getPort() << "--" << endl;
    cout << "\t\t\t -- request token " << token << endl;

    if(token == 'H') real_peerport = new_request->getPort();

    switch(token){
        case '1':
            cout << "\t\t Connection to server succeed My Id[" << new_request->getBody() << "]" << endl;
            break;

        case '2':
            cout << "\t\tInitiator get peer IP-Port" << endl;
            setPeerAddress(new_request->getBody());
            cout << "\t\t ===> My peer address [as received from server] " << getPeerIP() << ":" << getPeerPort() << endl;
            original = getPeerPort();
            cout << "\t\t -- Data --to--> Peer [received port]" << endl;
            SendStream("X"); // if initiator is public won't be received
//
//            for(uint16_t i = 0; i < 65535; i++) {
//                peer_address.sin_port = htons(i);
//                SendStream("X");
//            }

            SendStream("3" + getPeerIP() + "/" + original, false);
            break;

        case '3':
            cout << "\t\tReceiver get peer IP-Port" << endl;
            setPeerAddress(new_request->getBody());
            cout << "\t\t ===> My peer address [as received from server] " << getPeerIP() << ":" << getPeerPort() << endl;
            original = getPeerPort();
//
//            cout << "\t\t -- Data --to--> Peer [all ports]" << endl;
//
//            for(uint16_t i = 0; i < 65535; i++) {
//                peer_address.sin_port = htons(i);
//                SendStream("X");
//            }

            break;

        case '4':
            cout << "\t\tServer informs Receiver about hello" << endl; //receiver is behind NAT, didn't receive X
            SendStream("HELLOOO");
            SendStream("4" + getPeerIP() + "/" + original, false);
            break;

        case '5':
            cout << "\t\tInitiator sending stream" << endl;

            peer_address.sin_port = htons(stoi(real_peerport));
            SendStream("X");

            for(uint16_t i = 0; i < 65535; i++) { // initiator[public] tries all ports

                if(i == stoi(real_peerport)) cout << "Sending to real port now" << endl;

                peer_address.sin_port = htons(i);
                SendStream("X");
            }

            SendStream("5" + getPeerIP() + "/" + original, false);
//            SendStream("X");
//            SendStream("This is the sender");
//            SendStream("This is the sender again");
//            SendStream("This is the sender again and again");
//            SendStream("7sender:finish", false);
            break;

        case '6':
            cout << "\t\tReceiver sending stream" << endl;
//            SendStream("X");
//            SendStream("This is the receiver");
//            SendStream("This is the receiver again");
//            SendStream("This is the receiver again and again");
            SendStream("7receiver:finish", false);
            break;

        case 'X':
            cout << "\t\t Received X" << endl;
            changePeerAddress(new_request->getAddress());
            cout << "\t\t ===> My peer address [as received by 'X' sync msg]" << getPeerIP() << ":" << getPeerPort() << endl;
            SendStream("X");
            SendStream("X");
            SendStream("X");
            SendStream("X");
            SendStream("X");
            SendStream("This is a msg");
            SendStream("This is a msg");
            SendStream("This is a msg");
            break;

        default:
            cout << "\t\t " <<new_request->getCode() << new_request->getBody() << endl;
            break;
    }

    cout << "\t ---- END of handle new request -----" << endl << endl << endl;
}
