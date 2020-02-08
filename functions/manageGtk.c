#include <gtk/gtk.h>
#include <sys/stat.h>
#include "database.h"
#include "functions.h"

int isConnected(){
    const char * LOGIN_FILE = "./fms/user.bin";
    FILE * file = fopen(LOGIN_FILE, "rb");
    if (file != NULL)
    {
        if (verifyLoginFile(LOGIN_FILE) == 1)
        {
            return 1;
        }
    }
    return 0;
}



char * getEntryText(GtkWidget * entry){
    GtkEntryBuffer * buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    char * txt = (char *) gtk_entry_buffer_get_text(buffer);
}

GtkWidget * getMainPage(user ** user){
    GtkWidget * loginWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_default_size(GTK_WINDOW(loginWindow), 700, 400);
    gtk_window_set_title(GTK_WINDOW(loginWindow), "Connexion");
	gtk_window_set_position (GTK_WINDOW (loginWindow), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(loginWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    return loginWindow;
}

void verifyConnect(GtkWidget * button,GtkWidget * loginWindow){
    const char * LOGIN_FILE = "./fms/user.bin";
    char * username;
    char * password;
    // On récupère les enfants de la Window, donc uniquement le container
    GList * list = gtk_container_get_children(GTK_CONTAINER(loginWindow));
    GList * l;
    for (l = list; l != NULL; l = l->next){
        gpointer container = l->data;

        // On récupère les enfants du container
        GList * containerList = gtk_container_get_children(GTK_CONTAINER(container));
        GList * containerContent;
        u_int8_t firstEntry = 1;
        for(containerContent = containerList; containerContent != NULL; containerContent = containerContent->next ){
            gpointer element = containerContent->data;
            // Si l'enfant est une entry, on va le noter, on cherche notre username et notre password en l'occurence
            if(GTK_IS_ENTRY(element)){
                if(firstEntry){
                    username = getEntryText(element);
                    firstEntry = 0;
                }else{
                    password = getEntryText(element);
                }
            }
        }
    }

    if(verifyLogins(username,password)){
        // Création du fichier username
        createLoginFile(LOGIN_FILE, username, password);
        // Destruction de la fenêtre de Login
        gtk_widget_destroy(loginWindow);
        // Lancer la fenêtre principale
        user *user = createUserStruct(username, password);
        gtk_widget_show_all(getMainPage(&user));
    }else{
        // Lancer la fenêtre d'erreur
        printf("\n Username ou mot de passe incorrect\n");
    }
}

GtkWidget * getLoginPage(){
    GtkWidget * loginWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget * mainContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    GtkWidget * inputUsername = gtk_entry_new();
    GtkWidget * inputPassword = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(inputPassword),FALSE);

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