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
u_int8_t isFollowed(u_int8_t idSerie);
void changeStatusEpisode(GtkWidget * widget,gpointer idEpisode);
void changeStatusSerie(GtkWidget * widget,gpointer idSerie);
GtkWidget * getMySeriesWindow();
GtkWidget * getTooSeeWindow();
GtkWidget * getSearchWindow();
void showSearchedSeries(GtkWidget * widget,argsSearchStruct * arg);