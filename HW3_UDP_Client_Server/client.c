#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>

#define MAX_BYTES 1024

void send_datagram_to_server(char *ip_addr, int port);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./client IPAddress PortNumber (Example: ./client 127.0.0.1 5500)");
        return 1;
    } else {
        send_datagram_to_server(argv[1], atoi(argv[2]));
    }
    return 0;
}

void send_datagram_to_server(char *ip_addr, int port) {
    int sockfd;
    char buffer[MAX_BYTES];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip_addr);

    for (;;) {
        bzero(buffer, sizeof(buffer));
        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
        printf("Enter a string (or press Space to exit): ");

        int i = 0, n, len;
        while ((buffer[i] = getchar()) != '\n'){
            if (buffer[i] == 32) { // SPACE char
                return;
            }
            i++;
        } ;

        sendto(sockfd, (const char *)buffer, strlen(buffer),
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        printf("-> Message sent.\n");

        n = recvfrom(sockfd, (char *)buffer, MAX_BYTES,
                     MSG_WAITALL, (struct sockaddr *) &servaddr,
                     &len);
        buffer[n] = '\0';
        printf("-> Server respond: \n%s\n\n", buffer);
        if ((strncmp(buffer, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }

    // close the socket
    close(sockfd);
};