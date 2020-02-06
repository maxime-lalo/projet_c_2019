#include <stdlib.h>
#include <gtk/gtk.h>
#include <mysql.h>
#include "database.h"

int main(int argc,char **argv){
    MYSQL *conn = initBdd();
    
    GtkWidget* p_Window;
    GtkWidget* p_Label;

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