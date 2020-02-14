#include <gtk/gtk.h>
#include <sys/stat.h>
#include "database.h"
#include "functions.h"
#include "manageGtk.h"

int isConnected()
{
    const char *LOGIN_FILE = "./fms/user.bin";
    FILE *file = fopen(LOGIN_FILE, "rb");
    if (file != NULL)
    {
        if (verifyLoginFile(LOGIN_FILE) == 1)
        {
            return 1;
        }
    }
    return 0;
}
GtkWidget *getTooSeeWindow()
{
    GtkWidget *seriesWindow = GTK_WIDGET(gtk_scrolled_window_new(NULL, NULL));
    const char *LOGIN_FILE = "./fms/user.bin";
    char **userCred = getUserCred(LOGIN_FILE);
    user user = createUserStruct(userCred[0], userCred[1]);
    free(userCred);
    episodesNode * episodeToSee;
    char seasonEpiosdeString [511];
    user.series = getUserSeriesList(user.id);

    GtkWidget *seriesFlowBox = gtk_flow_box_new();
    gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(seriesFlowBox), 9);

    seriesNode *cursor;
    cursor = user.series;

    while (cursor != NULL)
    {
        episodeToSee = getLastNotSeenEpisode(user.id, cursor->serie.id);
        if (episodeToSee->episode.id != 0)
        {
            // On crée le container qui va contenir toutes les infos sur la série
            GtkBox *mainContainer = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));

            // On crée les boutons
            GtkButton *showButton = GTK_BUTTON(gtk_button_new());
            gtk_button_set_label(showButton, "Afficher");

            GtkButton *viewedButton = GTK_BUTTON(gtk_button_new());
            gtk_button_set_label(viewedButton, "Marquer comme vu");

            // On crée un label qui contiendra le nom de la série
            GtkLabel *serieNameLabel = GTK_LABEL(gtk_label_new(cursor->serie.name));

            GtkLabel *episodeNameLabel = GTK_LABEL(gtk_label_new(episodeToSee->episode.name));

            sprintf(seasonEpiosdeString, "S%dE%d", episodeToSee->episode.season, episodeToSee->episode.number);
            GtkLabel *seasonEpiosdeLabel = GTK_LABEL(gtk_label_new(seasonEpiosdeString));

            // On ajoute tout au container dans le bon ordre
            gtk_container_add(GTK_CONTAINER(mainContainer), GTK_WIDGET(getImage(cursor->serie.imageLink)));
            gtk_container_add(GTK_CONTAINER(mainContainer), GTK_WIDGET(serieNameLabel));
            gtk_container_add(GTK_CONTAINER(mainContainer), GTK_WIDGET(episodeNameLabel));
            gtk_container_add(GTK_CONTAINER(mainContainer), GTK_WIDGET(seasonEpiosdeLabel));
            gtk_container_add(GTK_CONTAINER(mainContainer), GTK_WIDGET(showButton));
            gtk_container_add(GTK_CONTAINER(mainContainer), GTK_WIDGET(viewedButton));

            // On link les signal aux boutons
            g_signal_connect(G_OBJECT(showButton), "clicked", G_CALLBACK(getSeriePage), GINT_TO_POINTER(cursor->serie.id));
            g_signal_connect(G_OBJECT(viewedButton), "clicked", G_CALLBACK(changeStatusEpisode), GINT_TO_POINTER(episodeToSee->episode.id));
            // On ajoute le tout au flowbox
            gtk_flow_box_insert(GTK_FLOW_BOX(seriesFlowBox), GTK_WIDGET(mainContainer), -1);
        }

        // On passe à la prochaine série contenu dans la liste chaînée
        cursor = cursor->next;
    }
    // On ajoute la flowbox à la window puis on return la window
    gtk_container_add(GTK_CONTAINER(seriesWindow), GTK_WIDGET(seriesFlowBox));
    return seriesWindow;
}
GtkWidget *getAgendaWindow()
{
    GtkWidget *agendaWindow = gtk_scrolled_window_new(NULL, NULL);

    const char *LOGIN_FILE = "./fms/user.bin";
    char **userCred = getUserCred(LOGIN_FILE);
    user user = createUserStruct(userCred[0], userCred[1]);
    free(userCred);

    GtkBox *agendaContainer = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    GtkBox *containerAgenda = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    for (uint8_t i = 0; i < 7; i++)
    {
        GtkWidget *separator = GTK_WIDGET(gtk_separator_new(GTK_ORIENTATION_VERTICAL));

        char request[500];
        sprintf(request, "SELECT DAYNAME(DATE(DATE_ADD(NOW(), INTERVAL %d DAY))) as name,DATE_FORMAT(DATE(DATE_ADD(NOW(), INTERVAL %d DAY)),%s) as dateEu,DATE(DATE_ADD(NOW(), INTERVAL %d DAY)) as dateUs", i, i, "\"%d/%m/%Y\"", i);
        MYSQL_ROW answer = fetchRow(request);

        char formatLabel[200];
        sprintf(formatLabel, "%s | %s", dayName(answer[0]), answer[1]);
        GtkLabel *labelDay = GTK_LABEL(gtk_label_new(formatLabel));
        gtk_container_add(GTK_CONTAINER(agendaContainer), GTK_WIDGET(labelDay));

        gtk_container_add(GTK_CONTAINER(agendaContainer), GTK_WIDGET(separator));

        char requestEpisode[500];
        sprintf(requestEpisode, "SELECT c.id,c.season,c.number,c.name,c.duration,c.id_serie,d.thumbnail,d.nom FROM user a INNER JOIN serie_user b ON a.id = b.id_user INNER JOIN episode c ON b.id_serie = c.id_serie INNER JOIN serie d ON b.id_serie = d.id WHERE air_date = '%s' AND a.id = %d", answer[2], user.id);

        MYSQL_RES *resultEpisode;
        fetchAllRows(requestEpisode, &resultEpisode);

        MYSQL_ROW rowEpisode;
        while ((rowEpisode = mysql_fetch_row(resultEpisode)) != NULL)
        {
            GtkBox *serieEpisodeContainer = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10));
            gtk_box_set_homogeneous(serieEpisodeContainer, TRUE);
            gtk_container_add(GTK_CONTAINER(serieEpisodeContainer), GTK_WIDGET(getImage(rowEpisode[6])));

            GtkLabel *serieName = GTK_LABEL(gtk_label_new(rowEpisode[7]));
            GtkLabel *episodeName = GTK_LABEL(gtk_label_new(rowEpisode[3]));

            char formattedString[200];
            sprintf(formattedString, "S%sE%s", rowEpisode[1], rowEpisode[2]);
            GtkLabel *seasonEpisodeNumber = GTK_LABEL(gtk_label_new(formattedString));
            /** 
             * Module pour les boutons
             */
            u_int8_t idEpisode = atoi(rowEpisode[0]);
            u_int8_t idSerie = atoi(rowEpisode[5]);
            GtkBox *containerButtons = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
            GtkButton *showButton = GTK_BUTTON(gtk_button_new());
            gtk_button_set_label(showButton, "Afficher la série");
            g_signal_connect(G_OBJECT(showButton), "clicked", G_CALLBACK(getSeriePage), GINT_TO_POINTER(idSerie));

            GtkButton *markViewed = GTK_BUTTON(gtk_button_new());
            if (isWatched(idEpisode))
            {
                gtk_button_set_label(markViewed, "Marquer comme non vu");
            }
            else
            {
                gtk_button_set_label(markViewed, "Marquer comme vu");
            }
            g_signal_connect(markViewed, "clicked", G_CALLBACK(changeStatusEpisode), GINT_TO_POINTER(idEpisode));

            gtk_container_add(GTK_CONTAINER(containerButtons), GTK_WIDGET(showButton));
            gtk_container_add(GTK_CONTAINER(containerButtons), GTK_WIDGET(markViewed));
            /**
             * Fin module boutons
             */

            gtk_container_add(GTK_CONTAINER(serieEpisodeContainer), GTK_WIDGET(serieName));
            gtk_container_add(GTK_CONTAINER(serieEpisodeContainer), GTK_WIDGET(episodeName));
            gtk_container_add(GTK_CONTAINER(serieEpisodeContainer), GTK_WIDGET(seasonEpisodeNumber));

            gtk_container_add(GTK_CONTAINER(serieEpisodeContainer), GTK_WIDGET(containerButtons));

            gtk_container_add(GTK_CONTAINER(agendaContainer), GTK_WIDGET(serieEpisodeContainer));
        }
    }
    gtk_container_add(GTK_CONTAINER(agendaWindow), GTK_WIDGET(agendaContainer));
    return agendaWindow;
}
char *getEntryText(GtkWidget *entry)
{
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    char *txt = (char *)gtk_entry_buffer_get_text(buffer);
}

void closeApp(GtkWidget *button, gpointer **closeAppArgs)
{
    seriesNode *series = (seriesNode *)closeAppArgs;
    freeEpisodesNodeList(&series->seasons->episodes);
    freeSeasonsNodeList(&series->seasons);
    freeSeriesNodeList(&series);
    gtk_main_quit();
}

void getSeriePage(GtkWidget *button, gpointer idSerie)
{
    serie serie = getSerieStruct(GPOINTER_TO_INT(idSerie));
    GtkWidget *serieWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *serieContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(serieContainer), getImage(serie.imageLink));

    seasonsNode *cursor = getSerieSeasonsList(GPOINTER_TO_INT(idSerie));
    GtkComboBox *listSeasons = GTK_COMBO_BOX(gtk_combo_box_new());
    while (cursor != NULL)
    {
        char labelSeasonTxt[200];
        sprintf(labelSeasonTxt, "Saison %d", cursor->number);
        GtkLabel *labelSeason = GTK_LABEL(gtk_label_new(labelSeasonTxt));
        //gtk_list_box_insert(listSeasons,GTK_WIDGET(labelSeason),-1);
        cursor = cursor->next;
    }
    gtk_container_add(GTK_CONTAINER(serieContainer), GTK_WIDGET(listSeasons));
    gtk_window_set_default_size(GTK_WINDOW(serieWindow), 700, 289);
    gtk_window_set_title(GTK_WINDOW(serieWindow), (char *)serie.name);
    gtk_window_set_position(GTK_WINDOW(serieWindow), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(serieWindow), "destroy", G_CALLBACK(gtk_widget_hide_on_delete), serieWindow);
    gtk_container_add(GTK_CONTAINER(serieWindow), serieContainer);
    gtk_widget_show_all(serieWindow);
}

GtkWidget *getMainPage()
{
    const char *LOGIN_FILE = "./fms/user.bin";
    char **userCred = getUserCred(LOGIN_FILE);
    user user = createUserStruct(userCred[0], userCred[1]);
    free(userCred);
    user.series = getUserSeriesList(user.id);

    //Window principale + Box princiale (dans Window)
    GtkWidget *mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *mainContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    //Stack + Stack Switcher (permet de Switch entre les childs de Stack)
    GtkWidget *stack = gtk_stack_new();
    GtkStackSwitcher *switcher = GTK_STACK_SWITCHER(gtk_stack_switcher_new());

    /**
     * Début recherche
     */
    GtkWidget *allSeriesWindow = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *allSeriesFlowBox = gtk_flow_box_new();
    gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(allSeriesFlowBox), 6);
    gtk_container_add(GTK_CONTAINER(allSeriesWindow), allSeriesFlowBox);
    /**
     * Fin recherche
     */

    //Ajout/Création de boutons pour les différents stacks
    gtk_stack_add_titled(GTK_STACK(stack), GTK_WIDGET(getTooSeeWindow()), "series", "A voir");
    gtk_stack_add_titled(GTK_STACK(stack), GTK_WIDGET(getAgendaWindow()), "agenda", "Agenda");
    gtk_stack_add_titled(GTK_STACK(stack), GTK_WIDGET(allSeriesWindow), "allSeries", "Rechercher");

    //Alignements des boutons Stack Switcher
    gtk_widget_set_halign(GTK_WIDGET(switcher), GTK_ALIGN_CENTER);

    //Ajout de la struct Stack au Stack Switcher
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));

    //Configuration de base de la Window principale
    gtk_window_set_title(GTK_WINDOW(mainWindow), user.name);
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(closeApp), (gpointer *)&(user.series->serie));

    //Ajout du Stack Switcher et Stack au mainContainer
    gtk_box_pack_start(GTK_BOX(mainContainer), GTK_WIDGET(switcher), FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(mainContainer), stack, TRUE, TRUE, 0);

    //Ajout du mainContainer à la mainWindow
    gtk_container_add(GTK_CONTAINER(mainWindow), mainContainer);
    gtk_window_set_icon_from_file(GTK_WINDOW(mainWindow), "logo.png", NULL);
    gtk_window_maximize(GTK_WINDOW(mainWindow));
    return mainWindow;
}

void verifyConnect(GtkWidget *button, GtkWidget *loginWindow)
{
    const char *LOGIN_FILE = "./fms/user.bin";
    char *username;
    char *password;
    // On récupère les enfants de la Window, donc uniquement le container
    GList *list = gtk_container_get_children(GTK_CONTAINER(loginWindow));
    GList *l;
    for (l = list; l != NULL; l = l->next)
    {
        gpointer container = l->data;

        // On récupère les enfants du container
        GList *containerList = gtk_container_get_children(GTK_CONTAINER(container));
        GList *containerContent;
        u_int8_t firstEntry = 1;
        for (containerContent = containerList; containerContent != NULL; containerContent = containerContent->next)
        {
            gpointer element = containerContent->data;
            // Si l'enfant est une entry, on va le noter, on cherche notre username et notre password en l'occurence
            if (GTK_IS_ENTRY(element))
            {
                if (firstEntry)
                {
                    username = getEntryText(element);
                    firstEntry = 0;
                }
                else
                {
                    password = getEntryText(element);
                }
            }
        }
    }

    if (verifyLogins(username, password))
    {
        // Création du fichier username
        createLoginFile(LOGIN_FILE, username, password);
        // Destruction de la fenêtre de Login
        gtk_widget_hide_on_delete(loginWindow);
        // Lancer la fenêtre principale
        gtk_widget_show_all(getMainPage());
    }
    else
    {
        // Lancer la fenêtre d'erreur
        printf("\n Username ou mot de passe incorrect\n");
    }
}

GtkWidget *getLoginPage()
{
    GtkWidget *loginWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *mainContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *inputUsername = gtk_entry_new();
    GtkWidget *inputPassword = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(inputPassword), FALSE);

    GtkWidget *buttonSubmit = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(buttonSubmit), "Se connecter");
    g_signal_connect(G_OBJECT(buttonSubmit), "clicked", G_CALLBACK(verifyConnect), loginWindow);

    gtk_window_set_title(GTK_WINDOW(loginWindow), "Connexion");
    gtk_window_set_default_size(GTK_WINDOW(loginWindow), 260, 40);
    gtk_window_set_position(GTK_WINDOW(loginWindow), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(loginWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(mainContainer), gtk_label_new("Nom d'utilisateur"));
    gtk_container_add(GTK_CONTAINER(mainContainer), inputUsername);
    gtk_container_add(GTK_CONTAINER(mainContainer), gtk_label_new("Mot de passe"));
    gtk_container_add(GTK_CONTAINER(mainContainer), inputPassword);
    gtk_container_add(GTK_CONTAINER(mainContainer), buttonSubmit);

    gtk_container_add(GTK_CONTAINER(loginWindow), mainContainer);
    return loginWindow;
}

GtkWidget *getImage(char *imageLink)
{
    GtkWidget *imageSerie = gtk_image_new();
    char imageLinkInDirectory[200];
    sprintf(imageLinkInDirectory, "./fms/images/%s", imageLink);

    FILE *testImg;
    testImg = fopen(imageLink, "r");
    if (testImg)
    {
        fclose(testImg);
    }
    else
    {
        char imageOnlineLink[300];
        sprintf(imageOnlineLink, "https://eplp.fr/images/%s", imageLink);
        get_page(imageOnlineLink, imageLinkInDirectory);
    }
    gtk_image_set_from_file(GTK_IMAGE(imageSerie), imageLinkInDirectory);
    return imageSerie;
}

u_int8_t isWatched(u_int8_t idEpisode)
{
    const char *LOGIN_FILE = "./fms/user.bin";
    char **userCred = getUserCred(LOGIN_FILE);
    user user = createUserStruct(userCred[0], userCred[1]);
    free(userCred);

    char request[255];
    sprintf(request, "SELECT * FROM episode_user WHERE episode = %d AND user = %d", idEpisode, user.id);
    MYSQL_ROW row = fetchRow(request);

    if (row)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void changeStatusEpisode(GtkWidget *widget, gpointer idEpisode)
{
    const char *LOGIN_FILE = "./fms/user.bin";
    char **userCred = getUserCred(LOGIN_FILE);
    user user = createUserStruct(userCred[0], userCred[1]);
    free(userCred);

    char request[250];
    if (isWatched(GPOINTER_TO_INT(idEpisode)))
    {
        sprintf(request, "DELETE FROM episode_user WHERE episode = %d AND user = %d", GPOINTER_TO_INT(idEpisode), user.id);
        gtk_button_set_label(GTK_BUTTON(widget), "Marquer comme vu");
    }
    else
    {
        sprintf(request, "INSERT INTO episode_user (episode,user) VALUES (%d,%d)", GPOINTER_TO_INT(idEpisode), user.id);
        gtk_button_set_label(GTK_BUTTON(widget), "Marquer comme non vu");
    }
    gtk_widget_show(GTK_WIDGET(widget));
    sqlExecute(request);
}