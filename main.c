/*
 * gcc -DDBG main.c -o udpcl
 * echo -e '!@' | udpcl 127.0.0.1 49162 1 (old)
 * udpcl 'ahl' '' 127.0.0.1 49162 1
 */
#include "lib/util.h"
#include "lib/app.h"
#include "lib/udp.h"
#include "lib/timef.h"
#include "lib/acp/main.h"

int sock_fd = -1;
struct sockaddr_in addr_sr;
socklen_t addr_sr_len = sizeof addr_sr;

Peer peer_client = {.fd = &sock_fd, .addr_size = sizeof peer_client.addr};

int main(int argc, char** argv) {
    if (argc != 6) {
        fputs("usage: updcl command data hostaddress port rcv_timeout\n", stderr);
        return (EXIT_FAILURE);
    }
    int port = -1;
    int n = sscanf(argv[4], "%d", &port);
    if (n != 1) {
        fputs("second argument should be an integer\n", stderr);
        return (EXIT_FAILURE);
    }
    __time_t tmo;
    n = sscanf(argv[5], "%ld", &tmo);
    if (n != 1) {
        fputs("third argument should be an integer\n", stderr);
        return (EXIT_FAILURE);
    }
    if (!initClient(&sock_fd, tmo)) {
        fputs("initUDPClient: failed\n", stderr);
        return (EXIT_FAILURE);
    }
    if (!makeClientAddr(&peer_client.addr, argv[3], port)) {
        fputs("makeUDPClientAddr: failed\n", stderr);
        return (EXIT_FAILURE);
    }
    ACPRequest request;
    acp_requestInit(&request);
    acp_requestSetCmd(&request, argv[1]);
    acp_requestStrCat(&request, estostr(argv[2]));
    putchar('\n');
    int r=acp_requestSend(&request, &peer_client);
    if (!r) {
        fputs("failed to send buffer\n", stderr);
        return (EXIT_FAILURE);
    }
    puts("waiting for response...");
    char buf[ACP_BUFFER_MAX_SIZE];
    int found = 0;
    while (!found) {
        memset(buf, 0, sizeof buf);
        if (recvfrom(sock_fd, buf, sizeof buf, 0, NULL, NULL) >= 0) {
            int is_not_last;
            unsigned int seq;
            int n = sscanf(buf, "%u" ACP_DELIMITER_COLUMN_STR "%d" ACP_DELIMITER_BLOCK_STR, &seq, &is_not_last);
            if (n == 2) {
                int i;int block_count=0;
                for(i=0;i<ACP_BUFFER_MAX_SIZE;i++){
                    if(buf[i]==ACP_DELIMITER_BLOCK){
                        block_count++;
                    }
                    if(block_count==1){
                        putchar(buf[i]);
                    }
                    if(block_count>1){
                        break;
                    }
                }
                if(!is_not_last){
                    found=1;
                }
            } else {
                fputs("failed to parse buffer sequence block\n", stderr);
                break;
            }

        } else {
            perror("recvfrom()");
            break;
        }
    }
    return (EXIT_SUCCESS);
}

