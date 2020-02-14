#include <gtk/gtk.h>
#include "database.h"

int isConnected();
char * getEntryText(GtkWidget * entry);
GtkWidget * getLoginPage();
GtkWidget * getMainPage();
void getSeriePage(GtkWidget *button, gpointer idSerie);
void verifyConnect(GtkWidget * button,GtkWidget * window);
void errorWindow(char * errorMessage);
void closeWindow(GtkWidget * widget,GtkWidget * window);
GtkWidget * getImage(char * imageLink);
u_int8_t isWatched(u_int8_t idEpisode);
void changeStatusEpisode(GtkWidget * widget,gpointer idEpisode);
GtkWidget * getAgendaWindow();
GtkWidget * getTooSeeWindow();