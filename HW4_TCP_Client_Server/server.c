#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define MAX_BYTES 1024

void create_tcp_connection(int port);
void process(char *output, char *input, int size);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./server PortNumber (Example ./server 5500)");
        return 1;
    } else {
        create_tcp_connection(atoi(argv[1]));
    }
    return 0;
}

void create_tcp_connection(int port) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (struct sockaddr *) &cli, (socklen_t *) &len);
    if (connfd < 0) {
        printf("Server accept failed...\n");
        exit(0);
    }
    else
        printf("Server accept the Client...\n\n");

    char buff[MAX_BYTES];
    // infinite loop for connection
    for (;;) {
        bzero(buff, MAX_BYTES);

        // read the message from client and copy it into buffer
        read(connfd, buff, MAX_BYTES);

        int buff_size = 0;
        for (int i = 0; i < MAX_BYTES ; i++) {
            if (buff[i] != '\0') {
                // client request exit by press SPACE
                if (buff[i] == 32) { // SPACE char
                    printf("Client request exit -> Server close connection!\n");
                    close(sockfd);
                    return;
                }
                buff_size++;
            }
            else break;
        }

        printf("Client message: %s\n", buff);;

        char result[MAX_BYTES] = "";
        process(result, buff, buff_size - 1); // subtract 1 for "\n"
        write(connfd, result, MAX_BYTES);
    }
}


// size not include end_of_string char and "\n"
void process(char *output, char *input, int size) {
    char digit_str[500] = ""; int idx1 = 0;
    char alpha_str[500] = ""; int idx2 = 0;
    int i;

    for (i = 0; i < size; i++) {
        char cur = input[i];
        if (!isdigit(cur) && !isalpha(cur)) {
            strcat(output, "Error");
            return;
        }
        if (isdigit(cur)) {
            digit_str[idx1] = cur;
            idx1 ++;
        }
        if (isalpha(cur)) {
            alpha_str[idx2] = cur;
            idx2 ++;
        }
    }

    digit_str[idx1] = '\n';

    strcat(output, digit_str);
    strcat(output, alpha_str);
}