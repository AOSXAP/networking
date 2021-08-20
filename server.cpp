#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

void __SETHINTS(struct addrinfo &hints){
    hints.ai_family = AF_INET; //ipv4
        hints.ai_socktype = SOCK_STREAM; // TCP
    
    hints.ai_flags = AI_PASSIVE; //bind to the wildcard address
}

#define THROW() std::cout<<errno; return -1;


int main(){
    //prepare struct addrinfo *hints

    struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
    
    __SETHINTS(hints);

    std::cout << "Server started!";

    //fill res with needed info
    //0 = 127.0.0.1, loopback adress

    getaddrinfo(0 , "8080" , &hints , &res);

    int server_socket = socket(res->ai_family , 
                res->ai_socktype , res->ai_protocol);

    //do the binding 

    if(bind(server_socket , res -> ai_addr , res -> ai_addrlen) == -1){
        THROW();
    }

    freeaddrinfo(res);

    std::cout << "Binding successful \n";
    //listen

    listen(server_socket , 10);

    std::cout << "Waiting for connections \n";

    //accept incoming conections

    struct sockaddr_storage client_address;  //info of the connecting client
        socklen_t client_len = sizeof(client_address);   

    int client_socket = accept(server_socket , 
            (struct sockaddr*) &client_address , &client_len);

    if(!(client_socket >= 0)){
        std::cout<<"accept failed \n"; THROW();
    }

    //recive and send

    char address_buffer[100] , request[1024];    
    
        getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, 
            sizeof(address_buffer), 0, 0, NI_NUMERICHOST); // 1 = NI_NUMERICHOST (see hostname as IP)
             
    int bytes_received = recv(client_socket, request, 1024, 0);   
        
    std::cout<<request<<"\n";

    int bytes_sent = send(client_socket, "<h1>Welcome to this server</h1>" , 
        strlen("<h1>Welcome to this server</h1>"), 0);   


    close(client_socket);
}