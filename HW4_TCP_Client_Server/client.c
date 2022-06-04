#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>

#define MAX_BYTES 1024

void create_tcp_connection(char *ip_addr, int port);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./client IPAddress PortNumber (Example: ./client 127.0.0.1 5500)");
        return 1;
    } else {
        create_tcp_connection(argv[1], atoi(argv[2]));
    }
    return 0;
}

void create_tcp_connection(char *ip_addr, int port) {
    int sockfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip_addr);
    servaddr.sin_port = htons(port);

    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(0);
    }
    else
        printf("Connected to the server..\n\n");

    char buff[MAX_BYTES];
    // infinite loop for connection
    for (;;) {
        bzero(buff, MAX_BYTES);
        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
        printf("Enter a string (or press Space to exit): ");

        int i = 0;
        while ((buff[i] = getchar()) != '\n') {
            if (buff[i] == 32) { // SPACE char
                printf("You pressed SPACE! -> Client request exit!");
                write(sockfd, buff, MAX_BYTES);
                close(sockfd);
                return;
            }
            i++;
        }
        write(sockfd, buff, MAX_BYTES);
        bzero(buff, MAX_BYTES);
        read(sockfd, buff, MAX_BYTES);
        printf("Server response:\n%s\n\n", buff);
    }
};