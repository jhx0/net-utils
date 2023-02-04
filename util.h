#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#define _SILENT (void)

#define IP_BUF 15

#define CLEAR(buf, size) \
    memset(buf, '\0', size);

typedef unsigned int uint;

void usage(const char *prg, const char *text);
void version(const char *prg, const char *ver);
void error(const char *msg, const char *file);
void *xmalloc(uint size);
char *resolve_domain(const char *host);

void usage(const char *prg, const char *text) {
    char tmp[512] = { '\0' };

    sprintf(tmp, text, prg);

    _SILENT fprintf(stdout, "%s\n", tmp);
    exit(EXIT_SUCCESS);
}

void version(const char *prg, const char *ver) {
    _SILENT fprintf(stdout, "%s v%s\n", prg, ver);
    exit(EXIT_SUCCESS);
}

void error(const char *msg, const char *file) {
    _SILENT fprintf(stderr, "Error: [%s]: %s\n", file, msg);
    exit(EXIT_SUCCESS);
}

void *xmalloc(uint size) {
    void *tmp = NULL;

    tmp = malloc(size);
    if(!tmp) {
        error(strerror(errno), __FILE__);
    }

    return tmp;
}

char *resolve_domain(const char *host) {
    struct addrinfo hints, *res = NULL;
    char *ip = NULL;

    CLEAR(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int ret = getaddrinfo(host, "80", &hints, &res);
    if(ret)
        error(gai_strerror(ret), __FILE__);

    ip = xmalloc(sizeof(char) * IP_BUF);
    
    CLEAR(ip, IP_BUF);

    inet_ntop(AF_INET, &res->ai_addr->sa_data[2], ip, IP_BUF);

    return ip;
}

#endif /* UTIL_H */
