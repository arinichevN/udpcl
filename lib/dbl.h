#ifndef LIBPAS_DBL_H
#define LIBPAS_DBL_H

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sqlite3.h>
#include "app.h"
#include "main.h"

extern int db_open(const char *path, sqlite3 **db);

extern int db_exec(sqlite3 *db, char *q, int (*callback)(void*, int, char**, char**), void * data);

extern int db_getInt(int *item, sqlite3 *db, char *q);

#endif 

