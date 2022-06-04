#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define MAX_BYTES 1024

_Noreturn void listen_datagram_from_client(int port);
void process(char *output, char *input, int size);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./server PortNumber (Example ./server 5500)");
        return 1;
    } else {
        listen_datagram_from_client(atoi(argv[1]));
    }
    return 0;
}

_Noreturn void listen_datagram_from_client(int port) {
    int sockfd;
    char buffer[MAX_BYTES];
    struct sockaddr_in serv_addr, cli_addr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));

    // Filling server information
    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0 ) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    printf("Listening...\n");

    for (;;) {
        int len, n;

        len = sizeof(cli_addr);  //len is value/result

        n = recvfrom(sockfd, (char *)buffer, MAX_BYTES, MSG_WAITALL,
                     (struct sockaddr *) &cli_addr, &len);
        buffer[n] = '\0';
        printf("Client message: %s\n", buffer);

        char result[1024] = "";
        process(result, buffer, n - 1);

        sendto(sockfd, (const char *)result, strlen(result),MSG_CONFIRM,
               (const struct sockaddr *) &cli_addr,
               len);
        printf("Response sent.\n");
    }
}


// size not include end_of_string char
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
