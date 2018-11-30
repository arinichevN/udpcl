
#include "acp/main.h"
#include "configl.h"

int config_checkPeerList(const PeerList *list) {
    //unique id
    for (int i = 0; i < list->length; i++) {
        for (int j = i + 1; j < list->length; j++) {
            if (strcmp(list->item[i].id, list->item[j].id) == 0) {
                fprintf(stderr, "checkPeerList: id = %s is not unique\n", list->item[i].id);
                return 0;
            }
        }
    }
    return 1;
}

int config_checkSensorFTSList(const SensorFTSList *list) {
    //unique id
    for (int i = 0; i < list->length; i++) {
        for (int j = i + 1; j < list->length; j++) {
            if (list->item[i].id == list->item[j].id) {
                fprintf(stderr, "config_checkSensorFTSList: id = %d is not unique\n", list->item[i].id);
                return 0;
            }
        }
    }
    return 1;
}

int config_checkEMList(const EMList *list) {
    //unique id
    for (int i = 0; i < list->length; i++) {
        for (int j = i + 1; j < list->length; j++) {
            if (list->item[i].id == list->item[j].id) {
                fprintf(stderr, "config_checkEMList(): id = %d is not unique\n", list->item[i].id);
                return 0;
            }
        }
    }
    return 1;
}
#define PDLd peer_data->fd
#define PDLi peer_data->list->item[peer_data->list->length]

static int getPeerList_callback(void *data, int argc, char **argv, char **azColName) {
    PeerData *peer_data = data;
    int c = 0;
    DB_FOREACH_COLUMN {
        if (DB_COLUMN_IS("id")) {
            strcpyma(&PDLi.id, DB_COLUMN_VALUE);
            if(PDLi.id==NULL){
                peer_data->list->length++;
                return EXIT_FAILURE;
            }
            c++;
        } else if (DB_COLUMN_IS("port")) {
            PDLi.port = DB_CVI;
            c++;
        } else if (DB_COLUMN_IS("ip_addr")) {
            strcpyma(&PDLi.addr_str, DB_COLUMN_VALUE);
            if(PDLi.addr_str==NULL){
                peer_data->list->length++;
                return EXIT_FAILURE;
            }
            c++;
        } else {
            putsde("unknown column\n");
        }
    }
#define N 3
    if (c != N) {
        peer_data->list->length++;
        printde("required %d columns but %d found\n", N, c);
        return EXIT_FAILURE;
    }
#undef N
    if (!makeClientAddr(&PDLi.addr, PDLi.addr_str, PDLi.port)) {
        printde("bad ip address for peer with id=%s\n", PDLi.id);
        peer_data->list->length++;
        return EXIT_FAILURE;
    }
    PDLi.addr_size = sizeof PDLi.addr;
    PDLi.fd = PDLd;
    PDLi.active = 0;
    peer_data->list->length++;
    return EXIT_SUCCESS;
}

#undef PDLd
#undef PDLi

static int getPhoneNumber_callback(void *data, int argc, char **argv, char **azColName) {
    S1List *item = data;
    int c = 0;
    DB_FOREACH_COLUMN {
        if (DB_COLUMN_IS("value")) {
            memcpy(&item->item[item->length * LINE_SIZE], DB_COLUMN_VALUE, LINE_SIZE);
            c++;
        } else {
            putsde("unknown column\n");
        }
    }
    item->length++;
#define N 1
    if (c != N) {
        printde("required %d columns but %d found\n", N, c);
        return EXIT_FAILURE;
    }
#undef N
    return EXIT_SUCCESS;
}

static int getSensorFTS_callback(void *data, int argc, char **argv, char **azColName) {
    SensorFTSData *item = data;
    int c = 0;
    DB_FOREACH_COLUMN {
        if (DB_COLUMN_IS("peer_id")) {
            Peer *peer;
            LIST_GETBYIDSTR(peer,item->peer_list,DB_COLUMN_VALUE)
            if (peer == NULL) {
                return EXIT_FAILURE;
            }
            item->sensor->peer = *peer;
            c++;
        } else if (DB_COLUMN_IS("remote_id")) {
            item->sensor->remote_id = DB_CVI;
            c++;
        } else if (DB_COLUMN_IS("sensor_id")) {
            item->sensor->id = DB_CVI;
            c++;
        } else {
            putsde("unknown column\n");
        }
    }
#define N 3
    if (c != N) {
        printde("required %d columns but %d found\n", N, c);
        return EXIT_FAILURE;
    }
#undef N
    return EXIT_SUCCESS;
}

static int getSensorFTSList_callback(void *data, int argc, char **argv, char **azColName) {
    SensorFTSListData *d = data;
    int c = 0;
    DB_FOREACH_COLUMN {
        if (DB_COLUMN_IS("peer_id")) {
            Peer *peer;
            LIST_GETBYIDSTR(peer,d->peer_list,DB_COLUMN_VALUE)
            if (peer == NULL) {
                return EXIT_FAILURE;
            }
            d->list->item[d->list->length].peer = *peer;
            c++;
        } else if (DB_COLUMN_IS("remote_id")) {
            d->list->item[d->list->length].remote_id = DB_CVI;
            c++;
        } else if (DB_COLUMN_IS("sensor_id")) {
            d->list->item[d->list->length].id = DB_CVI;
            c++;
        } else {
            putsde("unknown column\n");
        }
    }
    d->list->length++;
#define N 3
    if (c != N) {
        printde("required %d columns but %d found\n", N, c);
        return EXIT_FAILURE;
    }
#undef N
    return EXIT_SUCCESS;
}

static int getEM_callback(void *data, int argc, char **argv, char **azColName) {
    EMData *item = data;
    int c = 0;
    DB_FOREACH_COLUMN {
        if (DB_COLUMN_IS("peer_id")) {
            Peer *peer;
            LIST_GETBYIDSTR(peer,item->peer_list,DB_COLUMN_VALUE)
            if (peer == NULL) {
                return EXIT_FAILURE;
            }
            item->em->peer = *peer;
            c++;
        } else if (DB_COLUMN_IS("remote_id")) {
            item->em->remote_id = DB_CVI;
            c++;
        } else if (DB_COLUMN_IS("pwm_rsl")) {
            item->em->pwm_rsl = DB_CVF;
            c++;
        } else {
            putsde("unknown column\n");
        }
    }
    item->em->last_output = 0.0f;
#define N 3
    if (c != N) {
        printde("required %d columns but %d found\n", N, c);
        return EXIT_FAILURE;
    }
#undef N
    return 0;
}

static int getEMList_callback(void *data, int argc, char **argv, char **azColName) {
    EMListData *d = data;
    int c = 0;
    DB_FOREACH_COLUMN {
        if (DB_COLUMN_IS("peer_id")) {
            Peer *peer;
            LIST_GETBYIDSTR(peer,d->peer_list,DB_COLUMN_VALUE)
            if (peer == NULL) {
                return EXIT_FAILURE;
            }
            d->list->item[d->list->length].peer = *peer;
            c++;
        } else if (DB_COLUMN_IS("remote_id")) {
            d->list->item[d->list->length].remote_id = DB_CVI;
            c++;
        } else if (DB_COLUMN_IS("em_id")) {
            d->list->item[d->list->length].id = DB_CVI;
            c++;
        } else if (DB_COLUMN_IS("pwm_rsl")) {
            d->list->item[d->list->length].pwm_rsl = DB_CVF;
            c++;
        } else {
            putsde("unknown column\n");
        }
    }
    d->list->length++;
#define N 4
    if (c != N) {
        printde("required %d columns but %d found\n", N, c);
        return EXIT_FAILURE;
    }
#undef N
    return 0;
}

int config_getPeerList(PeerList *list, int *fd, const char *db_path) {
    RESET_LIST(list)
    sqlite3 *db;
    if (!db_openR(db_path, &db)) {
        putsde("failed\n");
        return 0;
    }
    int n = 0;
    char *qn = "select count(*) FROM peer";
    db_getInt(&n, db, qn);
    if (n <= 0) {
        db_close(db);
        return 1;
    }
    ALLOC_LIST(list,n)
    if (list->max_length!=n) {
        putsde("failed to allocate memory\n");
        db_close(db);
        return 0;
    }
    PeerData data = {.list = list, .fd = fd};
    char *q = "select id, port, ip_addr FROM peer";
    if (!db_exec(db, q, getPeerList_callback, &data)) {
        putsde("failed\n");
        db_close(db);
        FREE_LIST(list);
        return 0;
    }
    db_close(db);
    if (!config_checkPeerList(list)) {
        FREE_LIST(list);
        return 0;
    }
    return 1;
}

int config_getSensorFTSList(SensorFTSList *list, PeerList *peer_list, const char *db_path) {
    RESET_LIST(list)
    sqlite3 *db;
    if (!db_openR(db_path, &db)) {
        putsde("failed\n");
        return 0;
    }
    int n = 0;
    char *qn = "select count(*) FROM sensor_mapping";
    db_getInt(&n, db, qn);
    if (n <= 0) {
        db_close(db);
        return 1;
    }
    ALLOC_LIST(list,n)
    if (list->max_length!=n) {
        putsde("failed to allocate memory\n");
        db_close(db);
        return 0;
    }
    SensorFTSListData data = {.list = list, .peer_list = peer_list};
    char *q = "select sensor_id, peer_id, remote_id FROM sensor_mapping";
    if (!db_exec(db, q, getSensorFTSList_callback, &data)) {
        putsde("failed\n");
        db_close(db);
        FREE_LIST(list);
        return 0;
    }
    db_close(db);
    if (!config_checkSensorFTSList(list)) {
        FREE_LIST(list);
        return 0;
    }
    return 1;
}

int config_getEMList(EMList *list, PeerList *peer_list, const char *db_path) {
    RESET_LIST(list)
    sqlite3 *db;
    if (!db_openR(db_path, &db)) {
        putsde("failed\n");
        return 0;
    }
    int n = 0;
    char *qn = "select count(*) FROM em_mapping";
    db_getInt(&n, db, qn);
    if (n <= 0) {
        db_close(db);
        return 1;
    }
    ALLOC_LIST(list,n)
    if (list->max_length!=n) {
        putsde("failed to allocate memory\n");
        db_close(db);
        return 0;
    }
    EMListData data = {.list = list, .peer_list = peer_list};
    char *q = "select em_id, peer_id, remote_id, pwm_rsl FROM em_mapping";
    if (!db_exec(db, q, getEMList_callback, &data)) {
        putsde("failed\n");
        db_close(db);
        FREE_LIST(list);
        return 0;
    }
    db_close(db);
    if (!config_checkEMList(list)) {
        FREE_LIST(list);
        return 0;
    }
    return 1;
}

int config_getSensorFTS(SensorFTS *item, int sensor_id, const PeerList *pl, sqlite3 *db) {
    char q[LINE_SIZE];
    SensorFTSData data = {item, pl};
    memset(item, 0, sizeof *item);
    snprintf(q, sizeof q, "select sensor_id, peer_id, remote_id from sensor_mapping where sensor_id=%d", sensor_id);
    if (!db_exec(db, q, getSensorFTS_callback, &data)) {
        putsde("failed\n");
        return 0;
    }
    item->id = sensor_id;
    return 1;
}

int config_getEM(EM *item, int em_id, const PeerList *pl, sqlite3 *db) {
    char q[LINE_SIZE];
    EMData data = {item, pl};
    memset(item, 0, sizeof *item);
    snprintf(q, sizeof q, "select peer_id, remote_id, pwm_rsl from em_mapping where em_id=%d", em_id);
    if (!db_exec(db, q, getEM_callback, &data)) {
        putsde("failed\n");
        return 0;
    }
    item->id = em_id;
    return 1;
}

int config_getPeer(Peer *item, const char * peer_id, int *fd, sqlite3 *dbl, const char *db_path) {
    if (dbl != NULL && db_path != NULL) {
        putsde("dbl xor db_path expected\n");
        return 0;
    }
    sqlite3 *db;
    if (db_path != NULL) {
        if (!db_openR(db_path, &db)) {
            putsde("failed\n");
            return 0;
        }
    } else {
        db = dbl;
    }
    char q[LINE_SIZE];
    PeerList pl = {.item = item, .length = 0, .max_length = 1};
    PeerData data = {.list = &pl, .fd = fd};
    memset(item, 0, sizeof *item);
    snprintf(q, sizeof q, "SELECT id, port, ip_addr FROM peer where id='%s'", peer_id);
    if (!db_exec(db, q, getPeerList_callback, &data)) {
        putsde("failed\n");
        if (db_path != NULL) {
            db_close(db);
        }
        return 0;
    }
    if (db_path != NULL) {
        db_close(db);
    }
    if (data.list->length != 1) {
        printde("can't get peer: %s\n", peer_id);
        return 0;
    }

    return 1;
}

int config_getPort(int *port, const char *peer_id, sqlite3 *dbl, const char *db_path) {
    if (dbl != NULL && db_path != NULL) {
        putsde("dbl xor db_path expected\n");
        return 0;
    }
    sqlite3 *db;
    if (db_path != NULL) {
        if (!db_openR(db_path, &db)) {
            putsde("failed\n");
            return 0;
        }
    } else {
        db = dbl;
    }
    char q[LINE_SIZE];
    int _port=-1;
    snprintf(q, sizeof q, "SELECT port FROM peer where id='%s'", peer_id);
    if (!db_getInt(&_port, db, q)) {
        putsde("failed\n");
        if (db_path != NULL) {
            db_close(db);
        }
        return 0;
    }
    if (db_path != NULL) {
        db_close(db);
    }
    *port=_port;
    return 1;
}

int config_getPhoneNumberListG(S1List *list, int group_id, const char *db_path) {
    RESET_LIST(list)
    sqlite3 *db;
    if (!db_openR(db_path, &db)) {
        putsde("failed\n");
        return 0;
    }
    char q[LINE_SIZE];
    int n = 0;
    snprintf(q, sizeof q, "select count(*) from phone_number where group_id=%d", group_id);
    if (!db_getInt(&n, db, q)) {
        putsde("failed\n");
        db_close(db);
        return 0;
    }
    if (n <= 0) {
        db_close(db);
        return 1;
    }
    size_t list_size = LINE_SIZE * n * sizeof *(list->item);
    list->item = malloc(list_size);
    if (list->item == NULL) {
        perrord("malloc()");
        db_close(db);
        return 0;
    }
    list->max_length = n;
    memset(list->item, 0, list_size);
    snprintf(q, sizeof q, "select value from phone_number where group_id=%d", group_id);
    if (!db_exec(db, q, getPhoneNumber_callback, list)) {
        putsde("failed\n");
        db_close(db);
        FREE_LIST(list);
        return 0;
    }
    if (list->length != n) {
        printde("bad length: %d < %d\n", list->length, n);
        db_close(db);
        FREE_LIST(list);
        return 0;
    }
    db_close(db);
    return 1;
}

int config_getPhoneNumberListO(S1List *list, const char *db_path) {
    RESET_LIST(list)
    sqlite3 *db;
    if (!db_openR(db_path, &db)) {
        putsde("failed\n");
        return 0;
    }
    int n = 0;
    if (!db_getInt(&n, db, "select count(*) from phone_number")) {
        putsde("failed\n");
        db_close(db);
        return 0;
    }
    if (n <= 0) {
        db_close(db);
        return 1;
    }
    size_t list_size = LINE_SIZE * n * sizeof *(list->item);
    list->item = malloc(list_size);
    if (list->item == NULL) {
        perrord("malloc()");
        db_close(db);
        return 0;
    }
    list->max_length = n;
    memset(list->item, 0, list_size);
    if (!db_exec(db, "select value from phone_number order by group_id", getPhoneNumber_callback, list)) {
        putsde("failed\n");
        db_close(db);
        FREE_LIST(list);
        return 0;
    }
    if (list->length != n) {
        printde("bad length: %d < %d\n", list->length, n);
        db_close(db);
        FREE_LIST(list);
        return 0;
    }
    db_close(db);
    return 1;
}
