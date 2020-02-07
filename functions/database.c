#include <mysql.h>
#include <stdio.h>

MYSQL * initBdd(){
    MYSQL * conn;
	char * server = "localhost";
	char * user = "max";
	char * password = "root";
	char * database = "projet_c";
	
	conn = mysql_init(NULL);
	
	/* Connect to database */
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		
	}
   return conn;
}

MYSQL_ROW fetch(char * request,MYSQL * conn){
    MYSQL_RES * res;
	MYSQL_ROW row;
	if (mysql_query(conn, request)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
   
	res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    return row;
}  

char * fetchColumn(char * request,MYSQL * conn){
    MYSQL_ROW row = fetch(request,conn);
    return row[0];
}   
