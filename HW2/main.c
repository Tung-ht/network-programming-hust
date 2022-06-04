#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>

int validate_number(char *str);
int validate_ip(char *ip);
void print_if_is_domain_name(char *domain_name);
void print_if_is_ip_addr(char *ip_addr);

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if (isdigit(argv[1][0])) {
            if (validate_ip(argv[1])) {
                print_if_is_ip_addr(argv[1]);
            } else {
                puts("\nInvalid address");
            }
        } else {
            print_if_is_domain_name(argv[1]);
        }
    } else if (argc > 2) {
        printf("Too many arguments supplied.\n");
    } else {
        printf("One argument expected.\n");
    }
    return 0;
}

void print_if_is_ip_addr(char *ip_addr) {
    struct hostent *host = gethostbyaddr(ip_addr, 4, AF_INET);
    if (host == NULL) {
        puts("\nNot found information");
        return;
    }

    printf("\nOfficial name: %s", host->h_name);
    printf("\nAlias name: ");
    for (int i = 0; host->h_aliases[i] != NULL; i++) {
        printf("\n%s", host->h_aliases[i]);
    }
}

void print_if_is_domain_name(char *domain_name) {
    struct hostent *host = gethostbyname(domain_name);
    if (host == NULL) {
        puts("\nNot found information");
        return;
    }

    char ip[100];
    struct in_addr **addr_list;
    addr_list = (struct in_addr **) host->h_addr_list;

    for (int i = 0; addr_list[i] != NULL; i++) {
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]));
    }
    printf("\nOfficial IP: %s", ip);
}

int validate_number(char *str) {
    while (*str) {
        if (!isdigit(*str)) { //if the character is not a number, return false
            return 0;
        }
        str++; //point to next character
    }
    return 1;
}

int validate_ip(char *ip) { //check whether the IP is valid or not
    int i, num, dots = 0;
    char *ptr;
    if (ip == NULL)
        return 0;
    ptr = strtok(ip, "."); //cut the string using dor delimiter
    if (ptr == NULL)
        return 0;
    while (ptr) {
        if (!validate_number(ptr)) //check whether the sub string is holding only number or not
            return 0;
        num = atoi(ptr); //convert substring to number
        if (num >= 0 && num <= 255) {
            ptr = strtok(NULL, "."); //cut the next part of the string
            if (ptr != NULL)
                dots++; //increase the dot count
        } else
            return 0;
    }
    if (dots != 3) //if the number of dots are not 3, return false
        return 0;
    return 1;
}
