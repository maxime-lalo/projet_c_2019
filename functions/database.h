#ifndef __database_h
#define __database_h

#include <mysql.h>
#include <stdio.h>

MYSQL * initBdd();
MYSQL_ROW fetch(char * request, MYSQL * conn);
char * fetchColumn(char * request, MYSQL * conn);

#endif // __database_hls
