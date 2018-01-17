//
// Created by oranngeoai on 1/12/18.
//

#include "Server.h"


Server* Server::s_instance = NULL;

Server::Server(){

}

Server* Server::instance(){
    if(!s_instance)
        s_instance = new Server;
    return s_instance;
}

void Server::setDescriptor(int descriptor){
    this->descriptor = descriptor;
    if(descriptor < 0) perror("FAILED TO CREATE SOCKET");
}

int Server::getDescriptor() const{
    return descriptor;
}

void Server::setAddress(){
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_NUM);
}

SOCKADDR_P Server::getAddress() const{
    return (struct sockaddr*) &address;
}

void Server::createSocket(int p){
    PORT_NUM = p;
    setDescriptor( socket(AF_INET, SOCK_DGRAM, 0) );
    setAddress();
    if( bind(getDescriptor(), getAddress(), sizeof(address)) < 0 )
        perror("BIND ERROR");
}

User* Server::getByIPORT(string ip, string port){
    pthread_mutex_lock(&users_mutex);
    cout << "\t\t Users Array" << endl;
    for(int i=0; i<current_users; i++){
        cout << "\t\t\tusers[" << i << "] IP:Port" << users[i]->getIP() << ":" << users[i]->getPort() << endl;
        if(ip == users[i]->getIP() /* && port == users[i]->getPort()*/){
            pthread_mutex_unlock(&users_mutex);
            return users[i];
        }
    }

    pthread_mutex_unlock(&users_mutex);
    return NULL;
}

Request* Server::AcceptRequest(){

    char buffer[1024];

    struct sockaddr_in cli_address;
    socklen_t l = sizeof(cli_address);

    if( recvfrom(getDescriptor(), buffer, 1023, 0, (struct sockaddr *)&cli_address, &l) < 0)
        perror("READ STREAM FAILED");

    string data(buffer);

    Request* new_request = new Request(cli_address, data);

    return new_request;
}


User* Server::createUser(Request* request){
    User* new_user = new User;
    new_user->setDescriptor(getDescriptor());
    new_user->setAddress(request->getAddress());
    return new_user;
}

void Server::addUser(User* new_user){

    pthread_mutex_lock(&users_mutex);
    users[current_users] = new_user;

    new_user->setIndx(current_users);
    new_user->setID(current_users);

    current_users++;
    pthread_mutex_unlock(&users_mutex);

    new_user->writeToClient("1" + to_string(new_user->getID()));
}

void Server::removeUser(User* rem_user){
    pthread_mutex_lock(&users_mutex);
    users[rem_user->getIndx()] = users[current_users];
    users[current_users]->setIndx(rem_user->getIndx());
    current_users--;
    pthread_mutex_unlock(&users_mutex);
    delete rem_user;
}


User* Server::isExist(int ID){
    pthread_mutex_lock(&users_mutex);

    for(int i=0; i<current_users; i++){
        if(ID == users[i]->getID()){
            pthread_mutex_unlock(&users_mutex);
            return users[i];
        }
    }

    pthread_mutex_unlock(&users_mutex);
    return NULL;
}

void Server::HandleRequest(Request* new_request){

    cout << endl << "\t-----Handle new request-----" << endl;

    char token = new_request->getCode();

    size_t del;
    string peer_ID, peer_IP, peer_PORT;
    User* peer = NULL;
    User* client = NULL;
    User* new_user = NULL;


    cout << "\t\t -- This is a request from " << new_request->getIP() << ":" << new_request->getPort() << "--" << endl;
    cout << "\t\t -- request token " << token << endl;

    switch(token){
        case '1':
            cout << "\t\tNew Connection To Server" << endl;
            new_user = createUser(new_request);
            addUser(new_user);
            break;

        case '2':
            cout << "\t\tInitiate Connection with peer" << endl;
            peer_ID = new_request->getBody();
            peer = isExist(stoi(peer_ID));
            if(peer != NULL){
                client = getByIPORT(new_request->getIP(), new_request->getPort());
                client->connectToPeer(peer);
            }
            break;

        case '3':
            cout << "\t\tAcknowledge from sender sending hello to receiver" << endl;
            del = new_request->getBody().find('/');

            peer_IP = new_request->getBody().substr(0, del);
            peer_PORT = new_request->getBody().substr(del + 1, new_request->getBody().length() - del - 1);

            peer = getByIPORT(peer_IP, peer_PORT);
            peer->writeToClient("4");
            break;

        case '4':
            cout << "\t\tAcknowledge from receiver sending hello to initiator" << endl;
            del = new_request->getBody().find('/');

            peer_IP = new_request->getBody().substr(0, del);
            peer_PORT = new_request->getBody().substr(del + 1, new_request->getBody().length() - del - 1);

            peer = getByIPORT(peer_IP, peer_PORT);
            peer->writeToClient("5");
            break;

        case '5':
            del = new_request->getBody().find('/');

            peer_IP = new_request->getBody().substr(0, del);
            peer_PORT = new_request->getBody().substr(del + 1, new_request->getBody().length() - del - 1);

            peer = getByIPORT(peer_IP, peer_PORT);
            peer->writeToClient("6");

            break;
        case '7':
            cout << new_request->getBody() << endl;
            client = getByIPORT(new_request->getIP(), new_request->getPort());
            client->writeToClient("8server:finish");
            break;

        default:
            cout << "invalid token" << endl;
            cout << "code :" << new_request->getCode() << endl;
            cout << "body :" << new_request->getBody() << endl;
            break;
    }
    cout << endl << "\t----- END of Handle new request-----" << endl << endl << endl;
}
