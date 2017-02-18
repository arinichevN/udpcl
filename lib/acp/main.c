
#include "main.h"

int acp_initBuf(char *buf, size_t buf_size) {
    if (buf_size < ACP_RESP_BUF_SIZE_MIN) {
#ifdef MODE_DEBUG
        fputs("acp_initBuf: not enough space buf\n", stderr);
#endif
        return 0;
    }
    memset(buf, 0, buf_size);
    buf[0] = '_';
    buf[1] = '_';
    buf[2] = '\n';
    return 1;
}

void acp_parsePackI1(char *buf, I1List *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    while (list->length < list_max_size) {
        int p0;
        if (sscanf(buff, "%d", &p0) != 1) {
            break;
        }
        list->item[list->length] = p0;
        list->length++;
        strnline(&buff);
    }
}

void acp_parsePackI2(char *buf, I2List *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    while (list->length < list_max_size) {
        int p0, p1;
        if (sscanf(buff, "%d_%d", &p0, &p1) != 2) {
            break;
        }
        list->item[list->length].p0 = p0;
        list->item[list->length].p1 = p1;
        list->length++;
        strnline(&buff);
    }
}

void acp_parsePackI3(char *buf, I3List *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    while (list->length < list_max_size) {
        int p0, p1, p2;
        if (sscanf(buff, "%d_%d_%d", &p0, &p1, &p2) != 3) {
            break;
        }
        list->item[list->length].p0 = p0;
        list->item[list->length].p1 = p1;
        list->item[list->length].p2 = p2;
        list->length++;
        strnline(&buff);
    }
}

void acp_parsePackF1(char *buf, F1List *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    while (list->length < list_max_size) {
        float p0;
        if (sscanf(buff, "%f", &p0) != 1) {
            break;
        }
        list->item[list->length] = p0;
        list->length++;
        strnline(&buff);
    }
}

void acp_parsePackI1F1(char *buf, I1F1List *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    while (list->length < list_max_size) {
        int p0;
        float p1;
        if (sscanf(buff, "%d_%f", &p0, &p1) != 2) {
            break;
        }
        list->item[list->length].p0 = p0;
        list->item[list->length].p1 = p1;
        list->length++;
        strnline(&buff);
    }
}

void acp_parsePackS1(char *buf, S1List *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    char format[LINE_SIZE];
    int n = sprintf(format, "%%%lus", NAME_SIZE);
    if (n <= 0) {
        return;
    }
    while (list->length < list_max_size) {
        char p0[NAME_SIZE];
        if (sscanf(buff, format, p0) != 1) {
            break;
        }
        strcpy(&list->item[list->length * NAME_SIZE], p0);
        list->length++;
        strnline(&buff);
    }
}

void acp_parsePackI1S1(char *buf, I1S1List *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    char format[LINE_SIZE];
    int n = sprintf(format, "%%d_%%%lus", NAME_SIZE);
    if (n <= 0) {
        return;
    }
    while (list->length < list_max_size) {
        int p0;
        char p1[NAME_SIZE];
        if (sscanf(buff, format, &p0, p1) != 2) {
            break;
        }
        list->item[list->length].p0 = p0;
        strcpy(list->item[list->length].p1, p1);
        list->length++;
        strnline(&buff);
    }
}

void acp_parsePackFTS(char *buf, FTSList *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    while (list->length < list_max_size) {
        int id, state;
        float temp;
        struct timespec tm;
        if (sscanf(buff, "%d_%f_%ld_%ld_%d", &id, &temp, &tm.tv_sec, &tm.tv_nsec, &state) != 5) {
            break;
        }
        list->item[list->length].id = id;
        list->item[list->length].value = temp;
        list->item[list->length].tm.tv_sec = tm.tv_sec;
        list->item[list->length].tm.tv_nsec = tm.tv_nsec;
        list->item[list->length].state = state;
        list->length++;
        strnline(&buff);
    }
}

void acp_parsePackS2(char *buf, S2List *list, size_t list_max_size) {
    char *buff = buf;
    list->length = 0;
    if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
        return;
    }
    strnline(&buff);
    char format[LINE_SIZE];
    int n = sprintf(format, "%%%lus_%%%lus", LINE_SIZE, LINE_SIZE);
    if (n <= 0) {
        return;
    }
    while (list->length < list_max_size) {
        char p0[NAME_SIZE];
        char p1[NAME_SIZE];
        if (sscanf(buff, format, p0, p1) != 1) {
            break;
        }
        strcpy(list->item[list->length].p0, p0);
        strcpy(list->item[list->length].p1, p1);
        list->length++;
        strnline(&buff);
    }
}

size_t acp_packlen(char *buf, size_t buf_size) {
    int i, state = 0;
    size_t n = 0;
    for (i = 0; i < buf_size; i++) {
        switch (state) {
            case 0:
                if (buf[i] == '\n') {
                    state = 1;
                }
                n++;
                break;
            case 1:
                if (buf[i] == '\n') {
                    state = 2;
                } else {
                    state = 0;
                }
                n++;
                break;
            case 2:
                n++;
                return n;

        }

    }
    return 0;
}

//requires buf after acp_initBuf()

int acp_bufAddHeader(char *buf, char qnf, char *cmd_str, size_t buf_size) {
    buf[0] = qnf;
    buf[1] = cmd_str[0];
    buf[2] = '\n';
    return 1;
}

int acp_bufAddFooter(char *buf, size_t buf_size) {
    uint8_t crc = 0;
    //no more then 2 '\n''s in the end
    int state;
    size_t i, p;
    for (i = 0, p = -1, state = 0; i < buf_size && buf[i] != '\0' && state != 2; i++) {
        switch (state) {
            case 0:
                if (buf[i] == '\n') {
                    state = 1;
                }
                break;
            case 1:
                if (buf[i] == '\n' && p == -1) {
                    p = i;
                    state = 2;
                } else {
                    p = -1;
                    state = 0;
                }
                break;
        }
    }
    size_t pack_len;
    if (p == -1 && state == 0) {//we will add two '\n''s
        pack_len = strlen(buf) + 3;
        if (pack_len >= buf_size) {
#ifdef MODE_DEBUG
            fputs("acp_bufAddFooter: not enough space for three char in buf\n", stderr);
#endif
            return 0;
        }
        buf[pack_len - 3] = '\n';
        buf[pack_len - 2] = '\n';
    } else if (p == -1 && state == 1) {//we will add one '\n'
        pack_len = strlen(buf) + 2;
        if (pack_len >= buf_size) {
#ifdef MODE_DEBUG
            fputs("acp_bufAddFooter: not enough space for three char in buf\n", stderr);
#endif
            return 0;
        }
        buf[pack_len - 2] = '\n';
    } else if (p != -1 && state == 2) {//no '\n''s required
        puts("no ns req");
        if (p + 1 < strlen(buf)) {//we have too many '\n''s, lets cut them
            puts("too many ns");
            buf[p + 1] = '\0';
        }
        pack_len = p + 2;
        if (pack_len >= buf_size) {
#ifdef MODE_DEBUG
            fputs("acp_bufAddFooter: not enough space for three char in buf\n", stderr);
#endif
            return 0;
        }
    }
    crc_update_by_str(&crc, buf);
    buf[pack_len - 1] = crc;
    return 1;
}

int acp_sendBuf(char *buf, size_t buf_size, const Peer *peer) {
    if (sendto(*(peer->fd), buf, acp_packlen(buf, buf_size), 0, (struct sockaddr*) (&peer->addr), peer->addr_size) < 0) {
#ifdef MODE_DEBUG
        perror("acp_sendBuf: error writing to socket");
#endif
        return 0;
    }
    return 1;
}
//requires buf after acp_initBuf()

int acp_sendBufArrPackI1List(char cmd, size_t buf_size, const I1List *data, const Peer *peer) {
    char q[LINE_SIZE], buf[buf_size];
    int i;
    if (!acp_initBuf(buf, buf_size)) {
        return 0;
    }
    char cmd_str[] = {cmd, '\0'};
    if (!acp_bufAddHeader(buf, ACP_QUANTIFIER_SPECIFIC, cmd_str, buf_size)) {
        return 0;
    }
    for (i = 0; i < data->length; i++) {
        snprintf(q, sizeof q, "%d\n", data->item[i]);
        if (bufCat(buf, q, buf_size) == NULL) {
            return 0;
        }
    }
    if (!acp_bufAddFooter(buf, buf_size)) {
        return 0;
    }
    if (!acp_sendBuf(buf, buf_size, peer)) {
        return 0;
    }
    return 1;
}

int acp_sendBufArrPackI2List(char cmd, size_t buf_size, const I2List *data, const Peer *peer) {
    char q[LINE_SIZE], buf[buf_size];
    int i;
    if (!acp_initBuf(buf, buf_size)) {
        return 0;
    }
    char cmd_str[] = {cmd, '\0'};
    if (!acp_bufAddHeader(buf, ACP_QUANTIFIER_SPECIFIC, cmd_str, buf_size)) {
        return 0;
    }
    for (i = 0; i < data->length; i++) {
        snprintf(q, sizeof q, "%d_%d\n", data->item[i].p0, data->item[i].p1);
        if (bufCat(buf, q, buf_size) == NULL) {
            return 0;
        }
    }
    if (!acp_bufAddFooter(buf, buf_size)) {
        return 0;
    }
    if (!acp_sendBuf(buf, buf_size, peer)) {
        return 0;
    }
    return 1;
}

void acp_sendStr(const char *s, uint8_t *crc, const Peer *peer) {
    if (sendto(*(peer->fd), s, strlen(s), 0, (struct sockaddr*) (&peer->addr), peer->addr_size) < 0) {
#ifdef MODE_DEBUG

        perror("sendStr: error writing to socket");
#endif
    }
    crc_update_by_str(crc, s);
}

void acp_sendFooter(int8_t crc, Peer *peer) {
    char s[] = "\n\n \n";
    s[2] = crc;
    if (sendto(*(peer->fd), s, strlen(s), 0, (struct sockaddr*) (&peer->addr), peer->addr_size) < 0) {
#ifdef MODE_DEBUG

        perror("sendFooter: error writing to socket");
#endif
    }
}

int acp_sendBufPack(char *buf, char qnf, char *cmd_str, size_t buf_size, const Peer *peer) {
    if (!acp_bufAddHeader(buf, qnf, cmd_str, buf_size)) {
#ifdef MODE_DEBUG
        fputs("acp_sendBufPack: acp_bufAddHeader() failed\n", stderr);
#endif
        return 0;
    }
    if (!acp_bufAddFooter(buf, buf_size)) {
#ifdef MODE_DEBUG
        fputs("acp_sendBufPack: acp_bufAddFooter() failed\n", stderr);
#endif
        return 0;
    }
    if (!acp_sendBuf(buf, buf_size, peer)) {
        return 0;
    }
    return 1;
}

int acp_sendStrPack(char qnf, char *cmd, size_t buf_size, const Peer *peer) {
    char buf[buf_size];
    memset(buf, 0, sizeof buf);
    if (!acp_bufAddHeader(buf, qnf, cmd, buf_size)) {
#ifdef MODE_DEBUG
        fputs("acp_sendStrPack: acp_bufAddHeader() failed\n", stderr);
#endif
        return 0;
    }
    if (!acp_bufAddFooter(buf, buf_size)) {
#ifdef MODE_DEBUG
        fputs("acp_sendStrPack: acp_bufAddFooter() failed\n", stderr);
#endif
        return 0;
    }
    if (!acp_sendBuf(buf, buf_size, peer)) {
        return 0;
    }
    return 1;
}

int acp_bufCatDate(struct tm *date, char *buf, size_t buf_size) {
    char q[LINE_SIZE];
    snprintf(q, sizeof q, "%d_%d_%d_%d_%d_%d\n",
            date->tm_year,
            date->tm_mon,
            date->tm_mday,
            date->tm_hour,
            date->tm_min,
            date->tm_sec
            );
    if (bufCat(buf, q, buf_size) == NULL) {
        return 0;
    }
    return 1;
}

int acp_recvOK(Peer *peer, size_t buf_size) {
    char buf[buf_size];
    if (recvfrom(*(peer->fd), buf, sizeof buf, 0, NULL, NULL) < 0) {
#ifdef MODE_DEBUG
        perror("acp_recvOK: recvfrom() error");
#endif
        return 0;
    }
    if (!crc_check(buf, sizeof buf)) {
#ifdef MODE_DEBUG
        fputs("acp_recvOK: crc_check() failed\n", stderr);
#endif
        return 0;
    }
    if (strncmp(buf + 1, ACP_RESP_REQUEST_SUCCEEDED, sizeof ACP_RESP_REQUEST_SUCCEEDED - 1) != 0) {
#ifdef MODE_DEBUG

        fprintf(stderr, "acp_recvOK: response is not OK but is: %c\n", buf[0]);
#endif
        return 0;
    }
    return 1;
}

char acp_recvPing(Peer *peer, size_t buf_size) {
    char buf[buf_size];
    if (recvfrom(*(peer->fd), buf, sizeof buf, 0, NULL, NULL) < 0) {
#ifdef MODE_DEBUG
        perror("acp_recvPing: recvfrom() error");
#endif
        return '\0';
    }
    if (!crc_check(buf, sizeof buf)) {
#ifdef MODE_DEBUG
        fputs("acp_recvPing: crc_check() failed\n", stderr);
#endif
        return '\0';
    }
    return buf[1];
}

char acp_recvInt(Peer *peer, size_t buf_size) {
    char buf[buf_size];
    if (recvfrom(*(peer->fd), buf, sizeof buf, 0, NULL, NULL) < 0) {
#ifdef MODE_DEBUG
        perror("acp_recvPing: recvfrom() error");
#endif
        return '\0';
    }
    if (!crc_check(buf, sizeof buf)) {
#ifdef MODE_DEBUG
        fputs("acp_recvPing: crc_check() failed\n", stderr);
#endif
        return '\0';
    }
    return buf[1];
}

int acp_recvFTS(FTSList *list, char qnf, char *cmd, size_t buf_size, size_t list_max_size, int fd) {
    char buf[buf_size];
    if (recvfrom(fd, buf, sizeof buf, 0, NULL, NULL) < 0) {
#ifdef MODE_DEBUG
        perror("acp_recvFTS: recvfrom() error");
#endif
        return 0;
    }
    if (strlen(buf) < ACP_RESP_BUF_SIZE_MIN) {
#ifdef MODE_DEBUG
        fputs("acp_recvFTS: not enough data\n", stderr);
#endif
        return 0;
    }
    if (buf[0] != qnf) {
#ifdef MODE_DEBUG
        fputs("acp_recvFTS: bad quantifier\n", stderr);
#endif
        return 0;
    }
    if (strncmp(&buf[1], cmd, 1) != 0) {
#ifdef MODE_DEBUG
        fputs("acp_recvFTS: bad command\n", stderr);
#endif
        return 0;
    }
    if (!crc_check(buf, sizeof buf)) {
#ifdef MODE_DEBUG
        fputs("acp_recvFTS: crc_check() failed\n", stderr);
#endif
        return 0;
    }
    acp_parsePackFTS(buf, list, list_max_size);
    return 1;
}

int acp_recvI2(I2List *list, char qnf, char *cmd, size_t buf_size, size_t list_max_size, int fd) {
    char buf[buf_size];
    if (recvfrom(fd, buf, sizeof buf, 0, NULL, NULL) < 0) {
#ifdef MODE_DEBUG
        perror("acp_recvFTS: recvfrom() error");
#endif
        return 0;
    }
    if (strlen(buf) < ACP_RESP_BUF_SIZE_MIN) {
#ifdef MODE_DEBUG
        fputs("acp_recvFTS: not enough data\n", stderr);
#endif
        return 0;
    }
    if (buf[0] != qnf) {
#ifdef MODE_DEBUG
        fputs("acp_recvFTS: bad quantifier\n", stderr);
#endif
        return 0;
    }
    if (strncmp(&buf[1], cmd, 1) != 0) {
#ifdef MODE_DEBUG
        fputs("acp_recvFTS: bad command\n", stderr);
#endif
        return 0;
    }
    if (!crc_check(buf, sizeof buf)) {
#ifdef MODE_DEBUG
        fputs("acp_recvFTS: crc_check() failed\n", stderr);
#endif
        return 0;
    }
    acp_parsePackI2(buf, list, list_max_size);
    return 1;
}

FUN_LOCK(SensorInt)

FUN_LOCK(SensorFTS)

FUN_LOCK(Peer)

FUN_LOCK(EM)

FUN_UNLOCK(SensorInt)

FUN_UNLOCK(SensorFTS)

FUN_UNLOCK(Peer)

FUN_UNLOCK(EM)

int acp_setEMOutput(EM *em, int output, size_t udp_buf_size) {
    if (lockEM(em)) {
        if (lockPeer(em->source)) {
            if (output == ((int) em->last_output)) {
                unlockPeer(em->source);
                unlockEM(em);
                return 0;
            }
            I2 di[1];
            di[0].p0 = em->remote_id;
            di[0].p1 = output;
            I2List data = {di, 1};
            if (!acp_sendBufArrPackI2List(ACP_CMD_SET_INT, udp_buf_size, &data, em->source)) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: acp_setEMOutput: failed to send request where em.id = %d\n", em->id);
#endif
                unlockPeer(em->source);
                unlockEM(em);
                return 0;
            }
            em->last_output = (float) output;
            unlockPeer(em->source);
            unlockEM(em);
            return 1;
        }
    }
}

int acp_setEMDutyCycle(EM *em, float output, size_t udp_buf_size) {
    if (lockEM(em)) {
        if (lockPeer(em->source)) {
            if (output == em->last_output) {
                unlockPeer(em->source);
                unlockEM(em);
                return 0;
            }
            I2 di[1];
            di[0].p0 = em->remote_id;
            di[0].p1 = (int) output;
            I2List data = {di, 1};
            if (!acp_sendBufArrPackI2List(ACP_CMD_SET_DUTY_CYCLE_PWM, udp_buf_size, &data, em->source)) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: acp_setEMDutyCycle: failed to send request where em.id = %d\n", em->id);
#endif
                unlockPeer(em->source);
                unlockEM(em);
                return 0;
            }
            em->last_output = output;
            unlockPeer(em->source);
            unlockEM(em);
            return 1;
        }
    }
}

int acp_setEMOutputR(EM *em, int output, size_t udp_buf_size) {
    if (lockEM(em)) {
        if (lockPeer(em->source)) {
            I2 di[1];
            di[0].p0 = em->remote_id;
            di[0].p1 = output;
            I2List data = {di, 1};
            if (!acp_sendBufArrPackI2List(ACP_CMD_SET_INT, udp_buf_size, &data, em->source)) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: acp_setEMOutput: failed to send request where em.id = %d\n", em->id);
#endif
                unlockPeer(em->source);
                unlockEM(em);
                return 0;
            }
            em->last_output = (float) output;
            unlockPeer(em->source);
            unlockEM(em);
            return 1;
        }
    }
}

int acp_setEMDutyCycleR(EM *em, float output, size_t udp_buf_size) {
    if (lockEM(em)) {
        if (lockPeer(em->source)) {
            I2 di[1];
            di[0].p0 = em->remote_id;
            di[0].p1 = (int) output;
            I2List data = {di, 1};
            if (!acp_sendBufArrPackI2List(ACP_CMD_SET_DUTY_CYCLE_PWM, udp_buf_size, &data, em->source)) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: acp_setEMDutyCycle: failed to send request where em.id = %d\n", em->id);
#endif
                unlockPeer(em->source);
                unlockEM(em);
                return 0;
            }
            em->last_output = output;
            unlockPeer(em->source);
            unlockEM(em);
            return 1;
        }
    }
}

int acp_readSensorInt(SensorInt *s, size_t udp_buf_size) {
    if (lockSensorInt(s)) {
        if (lockPeer(s->source)) {
            struct timespec now = getCurrentTime();
/*
            if (!timeHasPassed(s->interval_min, s->last_read_time, now)) {
                unlockPeer(s->source);
                unlockSensorInt(s);
                return s->last_return;
            }
*/

            s->source->active = 0;
            s->source->time1 = now;
            s->last_read_time = now;
            s->last_return = 0;

            int di[1];
            di[0] = s->remote_id;
            I1List data = {di, 1};
            if (!acp_sendBufArrPackI1List(ACP_CMD_GET_INT, udp_buf_size, &data, s->source)) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: acp_readSensorInt: acp_sendBufArrPackI1List failed where sensor.id = %d\n", s->id);
#endif
                unlockPeer(s->source);
                unlockSensorInt(s);
                return 0;
            }

            //waiting for response...
            I2 td[1];
            I2List tl = {td, 0};
            if (!acp_recvI2(&tl, ACP_QUANTIFIER_SPECIFIC, ACP_RESP_REQUEST_SUCCEEDED, udp_buf_size, 1, *(s->source->fd))) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: acp_readSensorInt: acp_recvI2() error where sensor.id = %d\n", s->id);
#endif
                unlockPeer(s->source);
                unlockSensorInt(s);
                return 0;
            }
            if (tl.item[0].p0 != s->remote_id) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: acp_readSensorInt: response: id is not requested one where sensor.id = %d\n", s->id);
#endif
                s->source->active = 1;
                unlockPeer(s->source);
                unlockSensorInt(s);
                return 0;
            }
            s->source->active = 1;
            s->value = tl.item[0].p1;
            s->last_return = 1;
            unlockPeer(s->source);
            unlockSensorInt(s);
            return 1;
        }
        unlockSensorInt(s);
    }
    return 0;
}

int acp_readSensorFTS(SensorFTS *s, size_t udp_buf_size) {
    if (lockSensorFTS(s)) {
        if (lockPeer(s->source)) {
            struct timespec now = getCurrentTime();
/*
            if (!timeHasPassed(s->interval_min, s->last_read_time, now)) {
                unlockPeer(s->source);
                unlockSensorFTS(s);
                return s->last_return;
            }
*/

            s->source->active = 0;
            s->source->time1 = now;
            s->last_read_time = now;
            s->last_return = 0;
            s->value.state = 0;

            int di[1];
            di[0] = s->remote_id;
            I1List data = {di, 1};
            if (!acp_sendBufArrPackI1List(ACP_CMD_GET_FTS, udp_buf_size, &data, s->source)) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: sensorRead: acp_sendBufArrPackI1List failed where sensor.id = %d\n", s->id);
#endif
                unlockPeer(s->source);
                unlockSensorFTS(s);
                return 0;
            }

            //waiting for response...
            FTS td[1];
            FTSList tl = {td, 0};
            if (!acp_recvFTS(&tl, ACP_QUANTIFIER_SPECIFIC, ACP_RESP_REQUEST_SUCCEEDED, udp_buf_size, 1, *(s->source->fd))) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: acp_readSensorFTS: acp_recvFTS() error where sensor.id = %d\n", s->id);
#endif
                unlockPeer(s->source);
                unlockSensorFTS(s);
                return 0;
            }
            if (tl.item[0].state != 1) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: sensorRead: response: temperature sensor state is bad where sensor.id = %d\n", s->id);
#endif
                s->source->active = 1;
                unlockPeer(s->source);
                unlockSensorFTS(s);
                return 0;
            }
            if (tl.item[0].id != s->remote_id) {
#ifdef MODE_DEBUG
                fprintf(stderr, "ERROR: sensorRead: response: peer returned id=%d but requested one was %d\n", tl.item[0].id, s->remote_id);
#endif
                s->source->active = 1;
                unlockPeer(s->source);
                unlockSensorFTS(s);
                return 0;
            }
            s->source->active = 1;
            s->value = tl.item[0];
            s->last_return = 1;
            unlockPeer(s->source);
            unlockSensorFTS(s);
            return 1;
        }
        unlockSensorFTS(s);
    }
    return 0;
}

void acp_pingPeer(Peer *item, size_t udp_buf_size) {
    if (lockPeer(item)) {
        item->active = 0;
        char cmd_str[1] = {ACP_CMD_APP_PING};
        if (!acp_sendStrPack(ACP_QUANTIFIER_BROADCAST, cmd_str, udp_buf_size, item)) {
#ifdef MODE_DEBUG
            fputs("WARNING: acp_pingPeer: acp_sendStrPack failed\n", stderr);
#endif
            item->time1 = getCurrentTime();
            unlockPeer(item);
            return;
        }
        //waiting for response...
        char resp[2] = {'\0', '\0'};
        resp[0] = acp_recvPing(item, udp_buf_size);
        if (strncmp(resp, ACP_RESP_APP_BUSY, 1) != 0) {
#ifdef MODE_DEBUG
            fputs("WARNING: acp_pingPeer: acp_recvPing() peer is not busy\n", stderr);
#endif
            item->time1 = getCurrentTime();
            unlockPeer(item);
            return;
        }
        item->active = 1;
        item->time1 = getCurrentTime();
        unlockPeer(item);
    }
}

void acp_pingPeerList(PeerList *list, struct timespec interval, struct timespec now, size_t udp_buf_size) {
    size_t i;
    FORL{
        if (timeHasPassed(interval, LIi.time1, now)) {
            acp_pingPeer(&LIi, udp_buf_size);
        }
    }
}

int acp_sendCmdGetInt(Peer *item, char* cmd, int *output, size_t udp_buf_size) {
    if (lockPeer(item)) {
        item->active = 0;
        if (!acp_sendStrPack(ACP_QUANTIFIER_BROADCAST, cmd, udp_buf_size, item)) {
#ifdef MODE_DEBUG
            fputs("WARNING: acp_sendCmdGetInt: acp_sendStrPack failed\n", stderr);
#endif
            item->time1 = getCurrentTime();
            unlockPeer(item);
            return 0;
        }
        //waiting for response...
        char buf[udp_buf_size];
        if (recvfrom(*(item->fd), buf, sizeof buf, 0, NULL, NULL) < 0) {
#ifdef MODE_DEBUG
            perror("acp_sendCmdGetInt: recvfrom() error");
#endif
            item->time1 = getCurrentTime();
            unlockPeer(item);
            return 0;
        }
        item->active = 1;
        item->time1 = getCurrentTime();
        unlockPeer(item);

        if (!crc_check(buf, sizeof buf)) {
#ifdef MODE_DEBUG
            fputs("acp_sendCmdGetInt: crc_check() failed\n", stderr);
#endif
            return 0;
        }
        char *buff = buf;
        if (strlen(buff) < ACP_RESP_BUF_SIZE_MIN) {
#ifdef MODE_DEBUG
            fputs("acp_sendCmdGetInt: ERROR: bad buff length\n", stderr);
#endif
            return 0;
        }
        strnline(&buff);
        int p0;
        if (sscanf(buff, "%d", &p0) != 1) {
#ifdef MODE_DEBUG
            fputs("acp_sendCmdGetInt: ERROR: failed to scan\n", stderr);
#endif
            return 0;
        }
        *output = p0;
        return 1;
    }
#ifdef MODE_DEBUG
    fputs("acp_sendCmdGetInt: ERROR: lock failed\n", stderr);
#endif
    return 0;
}

void acp_waitUnlock(Peer *item, char *cmd_unlock,char *cmd_check, unsigned int interval_us, size_t udp_buf_size) {
    char state = 1;
    while (1) {
        switch (state) {
            case 1:
#ifdef MODE_DEBUG
                puts("acp_waitUnlock: sending ping request to locker...");
#endif
                acp_pingPeer(item, udp_buf_size);
                if (item->active) {
                    state = 2;
                }
                break;
            case 2:
#ifdef MODE_DEBUG
                puts("acp_waitUnlock: sending unlock request to locker...");
#endif
                if (acp_sendStrPack(ACP_QUANTIFIER_BROADCAST, cmd_unlock, udp_buf_size, item)) {
                    state = 3;
                }
                break;
            case 3:
            {
#ifdef MODE_DEBUG
                puts("acp_waitUnlock: checking locker to be unlocked...");
#endif
                int locked=1;
                if (acp_sendCmdGetInt(item, cmd_check, &locked, udp_buf_size)) {
                    if (!locked) {
                        state = 4;
                    }
                }
                break;
            }
            case 4:
#ifdef MODE_DEBUG
                puts("acp_waitUnlock: done!");
#endif
                return;
                break;
        }
        delayUsIdle(interval_us);
    }
}

void freePeer(PeerList *list) {
    free(list->item);
    list->item = NULL;
    list->length = 0;
}

void acp_printI1(I1List *list) {
    size_t i;
    char q[LINE_SIZE];
    puts("I1List dump");
    puts("+-----------+");
    puts("|   item    |");
    puts("+-----------+");
    for (i = 0; i < list->length; i++) {

        snprintf(q, sizeof q, "|%11d|", list->item[i]);
        puts(q);
    }
    puts("+-----------+");
}

void acp_printI2(I2List *list) {
    size_t i;
    char q[LINE_SIZE];
    puts("I2List dump");
    puts("+-----------+-----------+");
    puts("|     p0    |     p1    |");
    puts("+-----------+-----------+");
    for (i = 0; i < list->length; i++) {

        snprintf(q, sizeof q, "|%11d|%11d|", list->item[i].p0, list->item[i].p1);
        puts(q);
    }
    puts("+-----------+-----------+");
}

void acp_printI3(I3List *list) {
    size_t i;
    char q[LINE_SIZE];
    puts("I3List dump");
    puts("+-----------+-----------+-----------+");
    puts("|     p0    |     p1    |     p2    |");
    puts("+-----------+-----------+-----------+");
    for (i = 0; i < list->length; i++) {
        snprintf(q, sizeof q, "|%11d|%11d|%11d|", list->item[i].p0, list->item[i].p1, list->item[i].p2);
        puts(q);
    }
    puts("+-----------+-----------+-----------+");
}