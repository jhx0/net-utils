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

#define PRG_NAME "getip"
#define PRG_VERSION "0.1"

#define DOMAIN "icanhazip.com"
#define GET "GET / HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n"

char *text = "\
USAGE: %s [-h]\n\
    -h   Show this help\n\
    -v   Show version\n\
";

extern int errno;

static void reverse(const char *src, char *dest) {
	uint j = 0;

	for(int i = strlen(src) - 1; i >= 0; i--) {
		dest[j] = src[i];
		j++;
	}
}

static char *get_ip(const char *rip) {
	char rev_ip[100] = { '\0' };
	char buf[1024] = {0};
	char *ip = NULL;
	char get_request[256] = { '\0' };
	struct sockaddr_in addr;
	int sock = 0;

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == -1)
		error(strerror(errno), __FILE__);

	addr.sin_family = AF_INET;
	inet_aton(rip, &addr.sin_addr.s_addr);
	addr.sin_port = htons(80);

	if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
		error(strerror(errno), __FILE__);
	}

	sprintf(get_request, GET, DOMAIN);

	write(sock, get_request, strlen(get_request));
	
	CLEAR(rev_ip, 100);

	while(read(sock, buf, 1023) != 0);

	for(int i = strlen(buf) - 2; i >= 0; i--) {
		if(buf[i] == 10 && buf[i - 1] == 13)
			break;

		strncat(rev_ip, &buf[i], 1);
	}

	ip = xmalloc(sizeof(char) * IP_BUF);

	CLEAR(ip, IP_BUF);

	reverse(rev_ip, ip);
	
	close(sock);

	return ip;
}

int main(int argc, char **argv) {
	char *rip = NULL;
	char *ip = NULL;

    if(argc == 2 && (strcmp(argv[1], "-h") == 0)) {
    	usage(PRG_NAME, text);
	} else if(argc == 2 && (strcmp(argv[1], "-v") == 0)) {
    	version(PRG_NAME, PRG_VERSION);
	}

	rip = resolve_domain(DOMAIN);
#ifdef DEBUG
	_SILENT fprintf(stdout, "[Info] Resolved IP (%s): %s\n", DOMAIN, rip);
#endif

	ip = get_ip(rip);

	free(rip);

	_SILENT fprintf(stdout, "Public IP: %s\n", ip);

	free(ip);

	return EXIT_SUCCESS;
}
