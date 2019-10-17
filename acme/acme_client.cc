#include <iostream>
#include <string.h>  
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 

using namespace std;

void PrintError(const string &message)
{
    cerr << message << endl;
    exit(EXIT_FAILURE);
}

int main()
{
    int socket_fd;
    struct sockaddr_in serv_addr;
    int port_number;
    char * IP_address;
    char client_message[256];
    int message_result;
    string order;

    // Set the IP address (IPv4)
    IP_address = new char [sizeof("127.0.0.1")];
    strcpy(IP_address,"127.0.0.1");  // could instead have copied "localhost"

    // Set the port number
    port_number = 1132;

    // Create the socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) 
        PrintError("ERROR opening socket");
    
    // clear out the server address to make sure no problems with binding
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // set the sockaddr_in struct appropriately
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_number);

    // Use the inet_pton function to convert the IP address to
    // binary
    if (inet_pton(AF_INET, IP_address, &serv_addr.sin_addr) < 0)
       PrintError("Unable to convert IP address to binary to put in serv_addr"); 

    // Connect to the server
    if (connect(socket_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        PrintError("unable to connect to server");
    
    //Header
    cout << "ACME Products Dog Food"   << endl;
    cout << "Small Bag  - Price : $5 " << endl;
    cout << "Medium Bag - Price : $10" << endl;
    cout << "Large Bag  - Price : $15" << endl;

    while(true){

        cout << "\nEnter the size bag you wish to order: small, medium, or large \nOr type stop to exit.\n" ;
        
        bzero(client_message,256);
        getline(cin, order); // read the line from standard input
        
        if(order == "stop")
            break;

        strcpy(client_message,order.c_str());

        // Write the message to the socket to send to the server
        message_result = write( socket_fd, 
                                client_message,
                                strlen(client_message)
                            );
    
        if (message_result < 0) 
            PrintError("unable to write to socket");

        // Read the return message from the server
        bzero(client_message,256);
        message_result = recv(socket_fd,client_message,255,0);
        if (message_result < 0) 
            PrintError("unable to read from socket");

        //Printing order status received from the socket
        cout << client_message << endl;

    }

//    close(socket_fd);    //  only close it if you dont want to do more
                           //  calling the server from a run of the client


    cout << "\nThank you for shopping with ACME." << endl;

    delete [] IP_address;  // return the memory to the heap

    return 0;
}


