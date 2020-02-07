#include <gtk/gtk.h>
#include "database.h"

int isConnected(){
    // Vérifier si l'utilisateur est connecté
    return 0;
}

char * getEntryText(GtkWidget * entry){
    GtkEntryBuffer * buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    char * txt = (char *) gtk_entry_buffer_get_text(buffer);
}

void verifyConnect(GtkWidget * button,GtkWidget * loginWindow){
    gtk_widget_hide(loginWindow);
}

GtkWidget * getLoginPage(){
    GtkWidget * loginWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget * mainContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    GtkWidget * inputUsername = gtk_entry_new();
    GtkWidget * inputPassword = gtk_entry_new();
    GtkWidget * buttonSubmit = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(buttonSubmit),"Se connecter");
    g_signal_connect(G_OBJECT(buttonSubmit),"clicked",G_CALLBACK(verifyConnect),loginWindow);

    gtk_window_set_title(GTK_WINDOW(loginWindow), "Connexion");
    gtk_window_set_default_size(GTK_WINDOW(loginWindow), 260, 40);
	gtk_window_set_position (GTK_WINDOW (loginWindow), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(loginWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(mainContainer),gtk_label_new("Nom d'utilisateur"));
    gtk_container_add(GTK_CONTAINER(mainContainer),inputUsername);
    gtk_container_add(GTK_CONTAINER(mainContainer),gtk_label_new("Mot de passe"));
    gtk_container_add(GTK_CONTAINER(mainContainer),inputPassword);
    gtk_container_add(GTK_CONTAINER(mainContainer),buttonSubmit);

    gtk_container_add(GTK_CONTAINER(loginWindow),mainContainer);
    return loginWindow;
}

GtkWidget * getMainPage(){
    
}