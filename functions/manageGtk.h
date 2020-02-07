#include <gtk/gtk.h>
#include "database.h"

typedef struct user{
    int id;
    char * name;
    char * password;
    char * email;
}user;

int isConnected();
char * getEntryText(GtkWidget * entry);
GtkWidget * getLoginPage();
GtkWidget * getMainPage();
void verifyConnect(GtkWidget * button,GtkWidget * window);