#include <stdlib.h>
#include <gtk/gtk.h>
#include <mysql.h>
#include <curl/curl.h>
#include "./functions/database.h"
#include "./functions/manageGtk.h"
#include "./functions/shows.h"
#include "./functions/functions.h"

int main(int argc,char **argv){
    const char * APP_FOLDER = "./fms";
    
    MYSQL *conn = initBdd();
    gtk_init(&argc,&argv);
    GtkWidget * window;
    
    appDirectoryCheck(APP_FOLDER);

    window = isConnected()?getMainPage():getLoginPage();
    gtk_widget_show_all(window);

    gtk_main();
    return EXIT_SUCCESS;
}