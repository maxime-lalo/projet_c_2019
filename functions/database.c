#include <mysql.h>
#include <stdio.h>

MYSQL * initBdd(){
    MYSQL * conn;
	const char * server = "localhost";
	const char * user = "max";
	const char * password = "root";
	const char * database = "projet_c";
	
	conn = mysql_init(NULL);
	
	/* Connect to database */
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		
	}
   return conn;
}

MYSQL_ROW fetchRow(char * request){
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

void fetchAllRows(char * request, MYSQL_RES ** res){
	MYSQL * conn = initBdd();
	if (mysql_query(conn, request)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	*res = mysql_use_result(conn);
} 

char * fetchColumn(char * request){
    MYSQL_ROW row = fetchRow(request);
	if (row != NULL){
		return row[0];
	}else{
		return NULL;
	}
    
}   
