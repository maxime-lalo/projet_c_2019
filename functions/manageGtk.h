#include <gtk/gtk.h>
#include "database.h"

int isConnected();
char * getEntryText(GtkWidget * entry);
GtkWidget * getLoginPage();
GtkWidget * getMainPage();
void getSeriePage(GtkWidget *button, gpointer **serie);
void verifyConnect(GtkWidget * button,GtkWidget * window);
void errorWindow(char * errorMessage);
void closeWindow(GtkWidget * widget,GtkWidget * window);