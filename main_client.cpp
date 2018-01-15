//
// Created by oranngeoai on 1/12/18.
//

#include "Client.h"


void* StartConnection(void* client_v){

    Client* client = (Client*)client_v;

    int ID;
    cout << "Enter the ID to connect to: " << endl;
    cin >> ID;
    client->SendStream("2" + to_string(ID), false);

    return NULL;
}

int main(int argc, char const *argv[])
{

    pthread_t pid1, pid2;
    Client C(atoi(argv[2]), atoi(argv[1]));
    int err;

    C.SendStream("1", false);
    err = pthread_create(&pid2, NULL, &StartConnection, &C);

    while(true){
        Request new_request = C.AcceptRequest();
        C.handleIncomingRequest(&new_request);
    }


    return 0;
}