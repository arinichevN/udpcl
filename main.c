/*
 * gcc -DDBG main.c -o udpcl
 * echo -e '!@' | udpcl 192.168.0.102 49161 1 //rio_ds18b20
 * echo -e '!@' | ./bin 192.168.1.2 49180 1 //regsmp
 * echo -e '!@' | ./bin 192.168.1.2 49181 1 //regstp
 * echo -e '!@' | ./bin 192.168.1.250 49183 1 //gwu74
 */

#include "lib/app.h"
#include "lib/udp.h"
#include "lib/timef.h"
#include "lib/acp/main.h"

#define BUFSIZE 508

int udp_fd = -1;
struct sockaddr_in addr_sr;
socklen_t addr_sr_len = sizeof addr_sr;

Peer peer_client = {.fd = &udp_fd, .addr_size = sizeof peer_client.addr};

int main(int argc, char** argv) {
    char buf[BUFSIZE];
    int c, i;
    acp_initBuf(buf, sizeof buf);
    i = 0;
    while ((c = fgetc(stdin)) != EOF) {
        if (i >= BUFSIZE) {
            fputs("buffer overflow while reading from stdin\n", stderr);
            return (EXIT_FAILURE);
        }
        buf[i] = (char) c;
        i++;
    }
   
    if (argc != 4) {
        fputs("usage: updcl hostaddress port rcv_timeout\n", stderr);
        return (EXIT_FAILURE);
    }
    int port = -1;
    int n = sscanf(argv[2], "%d", &port);
    if (n != 1) {
        fputs("second argument should be an integer\n", stderr);
        return (EXIT_FAILURE);
    }
    __time_t tmo;
    n = sscanf(argv[3], "%ld", &tmo);
    if (n != 1) {
        fputs("third argument should be an integer\n", stderr);
        return (EXIT_FAILURE);
    }
    if (!initUDPClient(&udp_fd, tmo)) {
        fputs("initUDPClient: failed\n", stderr);
        return (EXIT_FAILURE);
    }
    if (!makeUDPClientAddr(&peer_client.addr, argv[1], port)) {
        fputs("makeUDPClientAddr: failed\n", stderr);
        return (EXIT_FAILURE);
    }
    if (!acp_bufAddFooter(buf, sizeof buf)) {
        fputs("failed to add footer to buffer\n", stderr);
        return (EXIT_FAILURE);
    }
    dumpStr(buf);
    if (!acp_sendBuf(buf, sizeof buf, &peer_client)) {
        fputs("failed to add footer to buffer\n", stderr);
        return (EXIT_FAILURE);
    }
    acp_initBuf(buf, sizeof buf);
    puts("waiting for response...");
    int found = 0, state = 0;
    while (!found) {
        if (recvfrom(udp_fd, buf, sizeof buf, 0, NULL, NULL) >= 0) {
            for (i = 0; i < strlen(buf); i++) {
                switch (state) {
                    case 0:
                        if (buf[i] == '\n') {
                            state = 1;
                        }
                        break;
                    case 1:
                        if (buf[i] == '\n') {
                            found = 1;
                        } else {
                            state = 0;
                        }
                        break;
                }

            }
            fputs(buf, stdout);
            memset(buf, 0, sizeof buf);
        } else {
            perror("recvfrom()");
            break;
        }
    }
    return (EXIT_SUCCESS);
}

