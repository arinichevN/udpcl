#ifndef LIBPAS_ACP_MAIN_H
#define LIBPAS_ACP_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#include "app.h"
#include "cmd.h"
#include "../dstructure_auto.h"
#include "../app.h"
#include "../timef.h"
#include "../util.h"
#include "../udp.h"
#include "../crc.h"

#define ACP_RETRY_NUM 12

#define ACP_COMMAND_MAX_SIZE 16
#define ACP_DATA_MAX_SIZE 456
#define ACP_BUFFER_MAX_SIZE 508

#define ACP_NUM_STR_LENGTH 12

#define ACP_RESPONSE_BUF_SIZE_MIN 9UL
#define ACP_REQUEST_BUF_SIZE_MIN 7UL

#define ACP_LAST_PACK 0
#define ACP_MIDDLE_PACK 1

#define ACP_DELIMITER_COLUMN  '\t'
#define ACP_DELIMITER_COLUMN_STR  "\t"
#define ACP_DELIMITER_ROW  '\n'
#define ACP_DELIMITER_ROW_STR  "\n"
#define ACP_DELIMITER_BLOCK  '\r'
#define ACP_DELIMITER_BLOCK_STR "\r"
#define ACP_DELIMITER_PACKET  '\0'

//the same for request and response
#define ACP_BLOCK_IND_CRC 3

#define ACP_FLOAT_FORMAT "%.3f"

typedef struct {
    char id[NAME_SIZE];
    int *fd;
    struct sockaddr_in addr;
    socklen_t addr_size;
    int active;
    struct timespec time1;
    Mutex mutex;
} Peer;

DEF_LIST(Peer)

typedef struct {
    char cmd[ACP_COMMAND_MAX_SIZE];
    char data[ACP_DATA_MAX_SIZE];
    char buf[ACP_BUFFER_MAX_SIZE];
    unsigned int id;
    size_t cmd_size;
    size_t data_size;
    size_t buf_size;
    uint8_t crc;
} ACPRequest;

typedef struct {
    char data[ACP_DATA_MAX_SIZE];
    char buf[ACP_BUFFER_MAX_SIZE];
    unsigned int id;
    unsigned int seq;
    int is_not_last;
    size_t data_size;
    size_t buf_size;
    uint8_t crc;
    int last_is_ok; //last packet has been successfully sent (multi-response)
} ACPResponse;

typedef int I1;
DEF_LIST(I1)
DEF_FUN_LIST_INIT(I1)

typedef struct {
    int p0;
    int p1;
} I2;
DEF_LIST(I2)
DEF_FUN_LIST_INIT(I2)

typedef struct {
    int p0;
    int p1;
    int p2;
} I3;
DEF_LIST(I3)
DEF_FUN_LIST_INIT(I3)
        
typedef float F1;
DEF_LIST(F1)
DEF_FUN_LIST_INIT(F1)

typedef struct {
    int p0;
    float p1;
} I1F1;
DEF_LIST(I1F1)
DEF_FUN_LIST_INIT(I1F1)
        
typedef char S1;
DEF_LIST(S1)
DEF_FUN_LIST_INIT(S1)

typedef struct {
    int p0;
    char p1[LINE_SIZE];
} I1S1;
DEF_LIST(I1S1)
DEF_FUN_LIST_INIT(I1S1)

typedef struct {
    char p0[LINE_SIZE];
    char p1[LINE_SIZE];
} S2;
DEF_LIST(S2)
DEF_FUN_LIST_INIT(S2)

typedef struct {
    int id;
    float value;
    struct timespec tm;
    int state;
} FTS;
DEF_LIST(FTS)
DEF_FUN_LIST_INIT(FTS)

typedef struct {
    int id;
    int remote_id;
    Peer *source;
    int value;
    Mutex mutex;
    struct timespec last_read_time;
    struct timespec interval_min;
    int last_return;
} SensorInt;
DEF_LIST(SensorInt)
DEF_FUN_LIST_INIT(SensorInt)

typedef struct {
    int id;
    int remote_id;
    Peer *source;
    FTS value;
    Mutex mutex;
    struct timespec last_read_time;
    struct timespec interval_min;
    int last_return;
} SensorFTS;
DEF_LIST(SensorFTS)
DEF_FUN_LIST_INIT(SensorFTS)

typedef struct {
    int id;
    int remote_id;
    Peer *source;
    float last_output; //we will keep last output value in order not to repeat the same queries to peers
    float pwm_rsl; //max duty cycle value (see lib/pid.h PWM_RSL)
    Mutex mutex;
} EM; //executive mechanism
DEF_LIST(EM)
DEF_FUN_LIST_INIT(EM)
        
#define FUN_ACP_REQUEST_DATA_TO(T) void acp_requestDataTo ## T(ACPRequest *request, T *list, size_t list_max_size){\
acp_dataTo ## T(request->data, list, list_max_size);\
}

#define DEF_FUN_ACP_REQUEST_DATA_TO(T) extern void acp_requestDataTo ## T(ACPRequest *request, T *list, size_t list_max_size);

#define ACP_CMD_IS(V) acp_cmdcmp(&request, V)
#define ACP_REQUEST_CREATE ACPRequest request; acp_requestInit(&request);
#define ACP_RESPONSE_CREATE ACPResponse response; acp_responseInit(&response);
        
DEF_FUN_LIST_GET_BY_IDSTR(Peer)

DEF_FUN_LIST_GET_BY_ID(SensorFTS)

DEF_FUN_LIST_GET_BY_ID(EM)

DEF_FUN_LOCK(SensorInt)

DEF_FUN_LOCK(SensorFTS)

DEF_FUN_LOCK(Peer)

DEF_FUN_LOCK(EM)

DEF_FUN_UNLOCK(SensorInt)

DEF_FUN_UNLOCK(SensorFTS)

DEF_FUN_UNLOCK(Peer)

DEF_FUN_UNLOCK(EM)
        
extern int acp_responseStrCat(ACPResponse *item, const char *str) ;

extern int acp_requestStrCat(ACPRequest *item, const char *str) ;

extern void acp_responseInit(ACPResponse *item) ;

extern void acp_requestInit(ACPRequest *item) ;

extern int acp_responseCoopRequest(ACPResponse *response, ACPRequest *request) ;

extern int acp_requestRead(ACPRequest *item, Peer *peer) ;

extern int acp_responseRead(ACPResponse *item, Peer *peer) ;

extern int acp_requestCheck(ACPRequest *item) ;

extern int acp_responseCheck(ACPResponse *response, ACPRequest *request) ;

extern void acp_requestSetCmd(ACPRequest * item, const char *cmd) ;

extern void acp_responsePack(ACPResponse *item) ;

extern void acp_requestPack(ACPRequest *item) ;

extern int acp_responseSend(ACPResponse *response, Peer *peer) ;

extern int acp_requestSend(ACPRequest *request, Peer *peer) ;

extern int acp_requestSendCmd(const char *cmd, ACPRequest *request, Peer *peer) ;

extern int acp_requestSendUnrequitedCmd(const char *cmd, Peer *peer) ;

extern int acp_requestSendI1List(char *cmd, const I1List *data, ACPRequest *request, Peer *peer) ;

extern int acp_requestSendI2List(char *cmd, const I2List *data, ACPRequest *request, Peer *peer) ;

extern int acp_requestSendS2List(char *cmd, const S2List *data, ACPRequest *request, Peer *peer) ;

extern int acp_requestSendS1List(char *cmd, const S1List *data, ACPRequest *request, Peer *peer) ;

extern int acp_requestSendUnrequitedI1List(char *cmd, const I1List *data, Peer *peer) ;

extern int acp_requestSendUnrequitedI2List(char *cmd, const I2List *data, Peer *peer) ;

extern int acp_requestSendUnrequitedS2List(char *cmd, const S2List *data, Peer *peer) ;

extern int acp_requestSendUnrequitedS1List(char *cmd, const S1List *data, Peer *peer) ;

extern void acp_responseSendStr(const char *s, int is_not_last, ACPResponse *response, Peer *peer) ;

extern int acp_responseReadFTSList(FTSList *list, size_t list_max_size, ACPRequest *request, Peer *peer) ;

extern int acp_responseReadI2List(I2List *list, size_t list_max_size, ACPRequest *request, Peer *peer) ;

extern int acp_setEMOutput(EM *em, int output) ;

extern int acp_setEMDutyCycle(EM *em, float output) ;

extern int acp_setEMOutputR(EM *em, int output) ;

extern int acp_setEMDutyCycleR(EM *em, float output) ;

extern int acp_readSensorInt(SensorInt *s) ;

extern int acp_readSensorFTS(SensorFTS *s) ;

extern void acp_pingPeer(Peer *item) ;

extern void acp_pingPeerList(PeerList *list, struct timespec interval, struct timespec now) ;

extern int acp_responseSendCurTime(ACPResponse *item, Peer *peer) ;

extern int acp_sendCmdGetInt(Peer *peer, char* cmd, int *output) ;

extern int acp_responseFTSCat(int id, float value, struct timespec tm, int state, ACPResponse *response) ;

extern int acp_responseITSCat(int id, int value, struct timespec tm, int state, ACPResponse *response);

extern void freePeer(PeerList *list) ;

extern int acp_cmdcmp(ACPRequest *request, char * cmd) ;

extern void acp_printI1(I1List *list) ;

extern void acp_printI2(I2List *list) ;

extern void acp_printI3(I3List *list) ;

DEF_FUN_ACP_REQUEST_DATA_TO(I1List)


DEF_FUN_ACP_REQUEST_DATA_TO(I2List)


DEF_FUN_ACP_REQUEST_DATA_TO(I3List)


DEF_FUN_ACP_REQUEST_DATA_TO(F1List)


DEF_FUN_ACP_REQUEST_DATA_TO(I1F1List)


DEF_FUN_ACP_REQUEST_DATA_TO(S1List)


DEF_FUN_ACP_REQUEST_DATA_TO(I1S1List)


DEF_FUN_ACP_REQUEST_DATA_TO(FTSList)


DEF_FUN_ACP_REQUEST_DATA_TO(S2List)

#endif 

