#include <stdlib.h>
#include <mysql.h>
#include <gtk/gtk.h>

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

int main(int argc,char **argv)
{
    MYSQL *conn = initBdd();
    
    GtkWidget* p_Window;
    GtkWidget* p_Label;
	gchar* sUtf8;

    gtk_init(&argc,&argv);

    p_Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(p_Window), "GTK+ 3.0");
    gtk_window_set_default_size(GTK_WINDOW(p_Window), 260, 40);
	gtk_window_set_position (GTK_WINDOW (p_Window), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(p_Window), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    char * name = fetchColumn("SELECT name FROM test",conn);
    p_Label=gtk_label_new(name);

    gtk_container_add(GTK_CONTAINER(p_Window), p_Label);

    gtk_widget_show_all(p_Window);
    
    gtk_main();
    return EXIT_SUCCESS;
}