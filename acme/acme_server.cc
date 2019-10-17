#include <iostream>
#include <string.h>   // partly to get bzero function to clear char arrays
#include <string>     // we're using C++ strings also
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void PrintError(const string &message)
{
    cerr << message << endl;
    exit(EXIT_FAILURE);
}

int main()
{
   int server_socket_fd, client_conn_fd;
   int client_addr_size;
   struct sockaddr_in server_addr, client_addr;
   int port_number;
   char message_from_client[256];
   char message_from_server_to_client[256];
   int client_message_length, server_message_length;
     
   // Set the port number
   port_number = 1132;

   // Create the socket for the server to listen on
   server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket_fd < 0) 
      PrintError("ERROR opening socket");

   // clear out the server address to make sure no problems with binding
   // if the address is clear then it won't think the address has
   // already been used by another socket
   bzero( (char *) &server_addr, sizeof(server_addr));

   // set the sockaddr_in struct appropriately
   // note that this assumes IPv4
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(port_number);

   // Bind the socket descriptor to the address
   if (bind(  server_socket_fd, 
               (struct sockaddr *) &server_addr,
               sizeof(server_addr)) 
                                      < 0) 
      PrintError("binding to socket failed");

   // Start the server listening on the socket
   // limit the number of connections in the listen queue to 3
   // (this is the backlog)
   listen(server_socket_fd,3);

   while (1) 
   {
      // clear out the client address to make sure no problems with accepting
      // on this address 
      // if the address is clear then it won't think the address has
      // already been used by another socket
      bzero( (char *) &client_addr, sizeof(client_addr));

      // accept a connection from a client 
      // on the open socket
      client_addr_size = sizeof(client_addr);
      client_conn_fd = accept(  server_socket_fd, 
                                 (struct sockaddr *) &client_addr, 
                                 (socklen_t *) &client_addr_size);
      if (client_conn_fd < 0) 
             PrintError("the accept failed");

      int small_price   = 5;
      int medium_price  = 10;
      int large_price   = 15;
      int small_count   = 5;
      int medium_count  = 3;
      int large_count   = 2;
      int total_funds   = 0;      
      string order_status;
      
      //Specific ACME dog food implementation 
      while(true)
      {
         bool order_validity = false;
         order_status = "Order Completed. Thank you";

         cout << "\nInventory"   << endl;
         cout << "Small Bags:"   << small_count    << endl;
         cout << "Medium Bags:"  << medium_count   << endl;
         cout << "Large Bags:"   << large_count    << endl;

         // Clear out the character array to store the client message
         // to make sure there's no garbage in it
         bzero(message_from_client,256);

         // read the message from the client (bag size client wants to order)
         client_message_length = recv( client_conn_fd,
                                       message_from_client,
                                       255,0);
         if (client_message_length < 0)
               PrintError("unable to read from socket");

         //Remove purchased bag from inventory
         if(strcmp(message_from_client,"small") == 0)
         {
            order_validity = true;
            if(small_count !=0)
            {  
               small_count--;
               total_funds = total_funds + 5;
               
            }
            else order_status = "We are currently out of small bags";
         }
         
         if(strcmp(message_from_client,"medium") == 0)
         {
            order_validity = true;
            if(medium_count != 0)
            {
               medium_count--;
               total_funds = total_funds + 10;
            }
            else order_status = "We are currently out of medium bags";
         }

         if(strcmp(message_from_client,"large") == 0)
         {
            order_validity = true;
            if(large_count != 0)
            {
            large_count--;
            total_funds = total_funds + 15;
            }
            else order_status = "We are currently out of large bags";
         }
         
         if(!order_validity)
            order_status = "You did not enter a correct size. Please try again.";

         bzero(message_from_server_to_client,256);
         order_status.copy(message_from_server_to_client, order_status.length() ); 

         //Writing message to socket
         server_message_length = write( client_conn_fd, 
                                       (char *) message_from_server_to_client,                                                   strlen(message_from_server_to_client)                                                   );
         if (server_message_length < 0) 
            PrintError("unable to write to socket");
         
         //Printing Funds
         cout << "Total Funds: $" << total_funds << endl;
      }

       sleep(5);  // let the client close first, avoids socket address reuse issues

       close(client_conn_fd);

    } // end while (1)
   
   close(server_socket_fd);strcpy(message_from_server_to_client, "We are currently out of small bags");
   return 0; 
}
