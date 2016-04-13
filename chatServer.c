#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void broadcast(char* msg, int* clients, int numClients);

int main(int argc, char** argv)
{
    uint16_t port = 3000;
    struct sockaddr_in* server = malloc(sizeof(struct sockaddr_in));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[10241];
    char* buf;
    if(sockfd == -1)  
    {
        puts("Bad Socket!!!!");
    }
    
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sin_port = htons( port );
    memset(server->sin_zero, '\0', sizeof server->sin_zero);
    
    int yes = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }
    socklen_t serverSize = sizeof(server);
    
    int bindfd = bind(sockfd, (struct sockaddr *)server, sizeof(struct sockaddr));
    if(bindfd < 0)
    {
        puts("Problem with binding...\n");
    }
    int listenfd;
    int clientfd;
    int MAX_CLIENTS = 1000;
    int* clients = malloc(MAX_CLIENTS * sizeof(int));
    int numberOfConnectedClients = 0;
    
    while(1)
    {
        listenfd = listen(sockfd, 100);
        printf("Listening....\n");
        clientfd = accept(sockfd, (struct sockaddr *) server, &serverSize);
        
        
        clients[numberOfConnectedClients++] = clientfd;
        char* message = "hello";
        broadcast(message, clients, numberOfConnectedClients);
        
        buffer[num] = '\0';
        printf("Server:Msg Received %s\n", buffer);
        if ((send(clientfd,buffer, strlen(buffer),0))== -1) 
        {
            fprintf(stderr, "Failure Sending Message\n");
            close(clientfd);
            break;
        }

        printf("Server:Msg being sent: %s\nNumber of bytes sent: %d\n",buffer, strlen(buffer));
    }
    
    
}

void broadcast(char* msg, int* clients, int numClients)
{
    int i;
    for(i = 0; i < numClients; i++)
    {
        send(*(clients + (i * sizeof(int))) , msg , strlen(msg) , 0);
        printf("sending to clients: %d", clients + (i * sizeof(int)));
    }
}
