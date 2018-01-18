//
// Created by oranngeoai on 1/12/18.
//

#include "Client.h"

void* StartConnection(void* client_v){

    Client* client = (Client*)client_v;

    int ID;
    cin >> ID;
    client->SendStream("2" + to_string(ID), false);

    return NULL;
}

void* readData(void* client_v){

    Client* client = (Client*)client_v;

    Request r = client->ReadStream();
    client->handleIncomingRequest(&r);

}

int main(int argc, char const *argv[])
{

    pthread_t pid_stun;
    pthread_t pid1, pid2;
    Client C(atoi(argv[1]), atoi(argv[2])/*, atoi(argv[3])*/);
    int err;

    C.SendStream("1", false);

    err = pthread_create(&pid1, NULL, &readData, &C);
    err = pthread_create(&pid2, NULL, &StartConnection, &C);


    while(true){
        Request new_request = C.AcceptRequest();
        C.handleIncomingRequest(&new_request);
    }


    return 0;
}