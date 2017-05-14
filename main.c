/*
 * gcc -DDBG main.c -o udpcl
 * echo -e '!@' | udpcl 127.0.0.1 49162 1
 */

#include "lib/app.h"
#include "lib/udp.h"
#include "lib/timef.h"
#include "lib/acp/main.h"

#define BUFSIZE 508

int sock_fd = -1;
struct sockaddr_in addr_sr;
socklen_t addr_sr_len = sizeof addr_sr;

Peer peer_client = {.fd = &sock_fd, .addr_size = sizeof peer_client.addr, .sock_buf_size=BUFSIZE};

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
    if (!initClient(&sock_fd, tmo)) {
        fputs("initUDPClient: failed\n", stderr);
        return (EXIT_FAILURE);
    }
    if (!makeClientAddr(&peer_client.addr, argv[1], port)) {
        fputs("makeUDPClientAddr: failed\n", stderr);
        return (EXIT_FAILURE);
    }
    if (!acp_bufAddFooter(buf, sizeof buf)) {
        fputs("failed to add footer to buffer\n", stderr);
        return (EXIT_FAILURE);
    }
    acp_dumpBuf(buf, sizeof buf);
    int r;
    r = acp_sendBuf(buf,  &peer_client);
    if (r < 0) {
        fputs("failed to send buffer\n", stderr);
        return (EXIT_FAILURE);
    }
    printf("%d bytes sent\n", r);
    acp_initBuf(buf, sizeof buf);
    puts("waiting for response...");
    int found = 0;
    while (!found) {
        memset(buf, 0, sizeof buf);
        if (recvfrom(sock_fd, buf, sizeof buf, 0, NULL, NULL) >= 0) {
            for (i = 0; i < BUFSIZE; i++) {
                if (buf[i] == ACP_DELIMITER_CRC) {
                    found = 1;
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

