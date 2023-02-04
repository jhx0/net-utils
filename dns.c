#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

#define PRG_NAME "dns"
#define PRG_VERSION "0.1"

#define PORT "80"

char *text = "\
USAGE: %s [-h] DOMAIN\n\
    -h   Show this help\n\
    -v   Show version\n\
";

static char *get_ip(const char *site) {
    struct addrinfo hints, *res = NULL;
    char *ip = NULL;

    CLEAR(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int ret = getaddrinfo(site, PORT, &hints, &res);
    if(ret) {
        error(gai_strerror(ret), __FILE__);
    }

    ip = malloc(sizeof(char) * IP_BUF);
    if(!ip) {
        error(strerror(errno), __FILE__);
    }
    
    CLEAR(ip, IP_BUF);

    if (inet_ntop(AF_INET, &res->ai_addr->sa_data[2], ip, IP_BUF) == NULL)
        inet_ntop(AF_INET, &res->ai_addr->sa_data[1], ip, IP_BUF);

    return ip;
}

int main(int argc, char **argv) {
    if(argc != 2)
        usage(PRG_NAME, text);

    if(argc == 2 && (strcmp(argv[1], "-h") == 0)) {
        usage(PRG_NAME, text);
    } else if(argc == 2 && (strcmp(argv[1], "-v") == 0)) {
        version(PRG_NAME, PRG_VERSION);
    }

    fprintf(stdout, "%s\n", get_ip(argv[1]));

    return EXIT_SUCCESS;
}
