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

MYSQL_ROW fetch(char * request){
	MYSQL * conn = initBdd();
    MYSQL_RES * res;
	MYSQL_ROW row;
	if (mysql_query(conn, request)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
   
	res = mysql_use_result(conn);
	if (res == NULL)
	{
		return NULL;
	}
	
    row = mysql_fetch_row(res);
	if(row == NULL){
		return NULL;
	}
    return row;
}  

char * fetchColumn(char * request){
    MYSQL_ROW row = fetch(request);
	if (row != NULL){
		return row[0];
	}else{
		return NULL;
	}
    
}   
