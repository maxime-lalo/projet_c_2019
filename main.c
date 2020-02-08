#include <stdlib.h>
#include <gtk/gtk.h>
#include <mysql.h>
#include "./functions/database.h"
#include "./functions/manageGtk.h"
#include "./functions/shows.h"
#include "./functions/functions.h"

int main(int argc,char **argv){
    const char * APP_FOLDER = "./fms";
    const char * LOGIN_FILE = "./fms/user.bin";
    
    MYSQL *conn = initBdd();
    gtk_init(&argc,&argv);
    GtkWidget * window;
    
    appDirectoryCheck(APP_FOLDER);

    if (isConnected())
    {
        printf("testMain");
        char ** userCreds = getUserCred(LOGIN_FILE);
        
        user * user = createUserStruct(userCreds[0], userCreds[1]);
        window = getMainPage(&user);
    }else
    {
        window = getLoginPage();
    }

    gtk_widget_show_all(window);

    gtk_main();
    return EXIT_SUCCESS;
}