CC=cc
CFLAGS=-Wall -Wextra -Wunused -pedantic -Wshadow -O2 -pipe

all: getip dns header

getip: getip.c
	$(CC) getip.c -o $@ $(CFLAGS)

dns: dns.c
	$(CC) dns.c -o $@ $(CFLAGS)

header: header.c
	$(CC) header.c -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f dns getip header *.o bin
