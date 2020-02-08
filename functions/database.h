#ifndef __database_h
#define __database_h

#include <mysql.h>
#include <stdio.h>

MYSQL * initBdd();
MYSQL_ROW fetch(char * request);
char * fetchColumn(char * request);

#endif // __database_hls
