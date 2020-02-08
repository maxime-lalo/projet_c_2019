#include <gtk/gtk.h>
#include "database.h"

int isConnected();
char * getEntryText(GtkWidget * entry);
GtkWidget * getLoginPage();
GtkWidget * getMainPage(user ** user);
void verifyConnect(GtkWidget * button,GtkWidget * window);