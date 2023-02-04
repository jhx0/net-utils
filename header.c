#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include "util.h"

#define PRG_NAME "header"
#define PRG_VERSION "0.1"

#define HEADER_END "\r\n\r\n"
#define GET "GET / HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n"

char *text = "\
USAGE: %s [-h] SITE\n\
    -h   Show this help\n\
    -v   Show version\n\
";

extern int errno;

static char *get_header(const char *rip) {
	char *header = NULL;
	char buf[1024] = {0};
	char get_request[256] = { '\0' };
	struct sockaddr_in addr;
	int sock = 0;

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == -1)
		error(strerror(errno), __FILE__);

	addr.sin_family = AF_INET;
	inet_aton(rip, (struct in_addr *)&addr.sin_addr.s_addr);
	addr.sin_port = htons(80);

	if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
		error(strerror(errno), __FILE__);
	}

	sprintf(get_request, GET, rip);

	write(sock, get_request, strlen(get_request));
	
	CLEAR(buf, 100);

	while(read(sock, buf, 1023) != 0);

	header = xmalloc(sizeof(char) * 512);
	CLEAR(header, sizeof(char) * 512);

    for(int i = 0; i <= (int)strlen(buf); i++) {
        if(strncmp(&buf[i], HEADER_END, strlen(HEADER_END)) == 0)
            break;
        else
			header[i] = buf[i];
    }
	
	close(sock);

	return header;
}

int main(int argc, char **argv) {
	char *ip = NULL;

	if(argc != 2) {
		usage(PRG_NAME, text);
	}

    if(argc == 2 && (strcmp(argv[1], "-h") == 0)) {
    	usage(PRG_NAME, text);
	} else if(argc == 2 && (strcmp(argv[1], "-v") == 0)) {
    	version(PRG_NAME, PRG_VERSION);
	}

	ip = resolve_domain(argv[1]);

	printf("%s\n", get_header(ip));

	free(ip);

	return 0;
}
