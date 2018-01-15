
#include "Server.h"

void* Work(void* user_v){

    Request* new_request = (Request*)new_request;

    cout << "passed to work request address = " << new_request << endl;

    Server::instance()->HandleRequest(new_request);
    cout << "deleting Request" << endl;
    delete new_request;
    return NULL;
}

int main(int argc, char const *argv[]) {

    pthread_t pid[100];
    int cnt = 0;


    Server* tcp = Server::instance();
    tcp->createSocket(atoi(argv[1]));

    cout << "SERVER LISTENING ON PORT " << argv[1] << endl;

    while(true){

        Request* new_request = tcp->AcceptRequest();
        int err = pthread_create(&pid[cnt++], NULL, &Work, new_request);

    }

    return 0;
}