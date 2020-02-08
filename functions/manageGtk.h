#include <gtk/gtk.h>
#include "database.h"

typedef struct user{
    int id;
    char * name;
    char * password;
    char * email;
}user;

char * getEntryText(GtkWidget * entry);
GtkWidget * getLoginPage();
GtkWidget * getMainPage();
void verifyConnect(GtkWidget * button,GtkWidget * window);
void errorWindow(char * errorMessage);
void closeWindow(GtkWidget * widget,GtkWidget * window);