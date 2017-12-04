

#include "udp.h"

int initServer(int *fd, int port) {
    struct sockaddr_in addr;
    if ((*fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
#ifdef MODE_DEBUG
        perror("initServer: socket()\n");
#endif
        return 0;
    }
    memset((char *) &addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(*fd, (struct sockaddr*) (&addr), sizeof (addr)) == -1) {
#ifdef MODE_DEBUG
        perror("initServer: bind()");
#endif
        freeSocketFd(fd);
        return 0;
    }
    return 1;
}

void freeSocketFd(int *udp_fd) {
    if (*udp_fd != -1) {
        if (close(*udp_fd) == -1) {
#ifdef MODE_DEBUG
            perror("freeSocketFd");
#endif
        }
        *udp_fd = -1;
    }
}

int initClient(int *fd, __time_t tmo) {
    if ((*fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
#ifdef MODE_DEBUG
        perror("initClient: socket()");
#endif
        return 0;
    }
    struct timeval tv = {tmo, 0};
    if (setsockopt(*fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv)) < 0) {
#ifdef MODE_DEBUG
        perror("initClient: timeout set failure");
#endif
        freeSocketFd(fd);
        return 0;
    }
    return 1;
}

int makeClientAddr(struct sockaddr_in *addr, const char *addr_str, int port) {
    memset(addr, 0, sizeof (*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (inet_aton(addr_str, &(addr->sin_addr)) == 0) {
#ifdef MODE_DEBUG
        perror("makeClientAddr: check address");
#endif
        return 0;
    }
    return 1;
}

int sendBuf(void *buf, size_t buf_size, int fd, struct sockaddr *addr, socklen_t addr_len) {
    ssize_t n;
    n=sendto(fd, buf, buf_size, 0, addr, addr_len);
    if (n < 0) {
#ifdef MODE_DEBUG
        perror("sendBuf");
#endif
        return 0;
    }
    return n;
}

int serverRead(void *buf, size_t buf_size, int fd, struct sockaddr * addr, socklen_t * addr_len) {
    if (recvfrom(fd, buf, buf_size, 0, addr, addr_len) < 0) {
#ifdef MODE_DEBUG
        perror("serverRead: recvfrom() error");
#endif
        return 0;
    }
    return 1;
}

int clientRead(int fd, void *buf, size_t buf_size) {
    if (recvfrom(fd, buf, buf_size, 0, NULL, NULL) < 0) {
#ifdef MODE_DEBUG
        perror("clientRead: recvfrom()");
#endif
        return 0;
    }
    return 1;
}
#define UDP_READ_SIZE 512

void readAll(int fd) {
    char buf[UDP_READ_SIZE];
    int r = 0;
    while (1) {
        r = recv(fd, (void *) buf, sizeof buf, 0);
        if (r < 0) {
#ifdef MODE_DEBUG
            perror("readAll: recvfrom()");
#endif
            return;
        }
        if (r < UDP_READ_SIZE) {
            return;
        }
    }

}