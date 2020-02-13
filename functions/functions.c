#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "database.h"
#include "functions.h"
#include <curl/curl.h>

episode getEpisode(int idEpisode){
    char request[294];
    char idEpisodeString[5];
    char temp[13];
    uint8_t i;
    MYSQL_ROW rowEpisode;
    MYSQL_RES *resultEpisode;
    episode episode;
    sprintf(idEpisodeString, "%d", idEpisode);

    strcat(strcat(strcpy(request, "SELECT * FROM episode WHERE id = \""), idEpisodeString), "\"");
    fetchAllRows(request, &resultEpisode);

    rowEpisode = mysql_fetch_row(resultEpisode);
    episode.id = atoi(rowEpisode[0]);
    episode.idSerie = atoi(rowEpisode[1]);
    episode.season = atoi(rowEpisode[2]);
    episode.number = atoi(rowEpisode[3]);
    strcpy(episode.name, rowEpisode[4]);
    strcpy(episode.shortDescription, rowEpisode[5]); 
    for(i=0; i<4; i++)temp[i] = rowEpisode[6][i];
    temp[4]= '\0';
    episode.airYear = atoi(temp);
    for(i=5; i<7; i++)temp[i] = rowEpisode[6][i];
    temp[7] = '\0';
    episode.airMonth = atoi(temp+5);
    for(i=8; i<10; i++)temp[i] = rowEpisode[6][i];
    temp[11] = '\0';
    episode.airDay = atoi(temp+8);
    episode.duration = atoi(rowEpisode[7]);
    return episode;
}

episodesNode * getSerieSeasonEpisodes(int idSerie, int seasonNum){
    char request[294];
    char idSerieString[5];
    char seasonNumString[5];
    MYSQL_ROW rowSeason;
    MYSQL_RES *resultSeason;
    episodesNode * episodeNodeInter = NULL;
    episodesNode * episodeNodeStart = NULL;

    sprintf(idSerieString, "%d", idSerie);
    sprintf(seasonNumString, "%d", seasonNum);

    strcat(strcat(strcat(strcat(strcpy(request, "SELECT id FROM episode WHERE id_serie = \""), idSerieString), "\" AND season =\""),seasonNumString),"\" ORDER BY number DESC");
    fetchAllRows(request, &resultSeason);

    while ((rowSeason = mysql_fetch_row(resultSeason)) != NULL)
    {
        episodeNodeInter = malloc(sizeof(episodesNode));
        episodeNodeInter->episode = getEpisode(atoi(rowSeason[0]));
        episodeNodeInter->next = episodeNodeStart;
        episodeNodeStart = episodeNodeInter; 
    }
    return episodeNodeStart;
}

seasonsNode * getSerieSeasonsList(int idSerie)
{
    char request[294];
    char idSerieString[5];
    int seasonNum = 1;
    char seasonNumString[5];
    MYSQL_ROW rowSeasons;
    MYSQL_RES *resultSerieSeasons;
    seasonsNode *seasonNodeStart = NULL;
    seasonsNode *seasonNodeInter = NULL;

    sprintf(idSerieString, "%d", idSerie);
    sprintf(seasonNumString, "%d", seasonNum);

    strcat(strcat(strcat(strcat(strcpy(request, "SELECT id FROM episode WHERE id_serie = \""), idSerieString), "\" AND season = \""),seasonNumString),"\" ORDER BY season DESC");
    fetchAllRows(request, &resultSerieSeasons);
    
    while ((rowSeasons = mysql_fetch_row(resultSerieSeasons)) != NULL)
    {
        //printf("test\n");
        seasonNodeInter = malloc(sizeof(seasonsNode));
        seasonNodeInter->episodes = getSerieSeasonEpisodes(idSerie, seasonNum);
        seasonNodeInter->number = seasonNum;
        seasonNodeInter->next = seasonNodeStart;
        seasonNodeStart = seasonNodeInter; 
        seasonNum ++;
        sprintf(seasonNumString, "%d", seasonNum);
        strcat(strcat(strcat(strcat(strcpy(request, "SELECT id FROM episode WHERE id_serie = \""), idSerieString), "\" AND season = \""),seasonNumString),"\"");
        fetchAllRows(request, &resultSerieSeasons);
    }
    return seasonNodeStart;
}

serie getSerieStruct(int idSerie)
{
    char request[294];
    char idSerieString[5];
    MYSQL_ROW rowSerie;
    MYSQL_RES *resultSerie;
    serie serie;
    //printf("test\n");
    sprintf(idSerieString, "%d", idSerie);
    strcat(strcat(strcpy(request, "SELECT * FROM serie WHERE id = \""), idSerieString), "\"");
    fetchAllRows(request, &resultSerie);
    rowSerie = mysql_fetch_row(resultSerie);
    serie.id = atoi(rowSerie[0]);
    strcpy(serie.name, rowSerie[1]);
    strcpy(serie.imageLink, rowSerie[2]);
    serie.state = atoi(rowSerie[3]);
    //printf("test\n");

    return serie;
}

seriesNode *getUserSeriesList(int idUser)
{
    char request[294];
    char idUserString[5];
    MYSQL_ROW rowSeries;
    MYSQL_RES *resultUserSeries;
    seriesNode *seriesNodeStart = NULL;
    seriesNode *seriesNodeInter = NULL;
    sprintf(idUserString, "%d", idUser);

    strcat(strcat(strcpy(request, "SELECT id_serie FROM serie_user WHERE id_user = \""), idUserString), "\"");
    fetchAllRows(request, &resultUserSeries);

    while ((rowSeries = mysql_fetch_row(resultUserSeries)) != NULL)
    {
        //printf("test\n");
        seriesNodeInter = malloc(sizeof(seriesNode));
        seriesNodeInter->serie = getSerieStruct(atoi(rowSeries[0]));
        seriesNodeInter->seasons = getSerieSeasonsList(atoi(rowSeries[0]));
        seriesNodeInter->next = seriesNodeStart;
        seriesNodeStart = seriesNodeInter;
    }
    
    return seriesNodeStart;
}

void addUserSeries(user *user)
{
    user->series = getUserSeriesList(user->id);
}

user createUserStruct(const char *username, const char *password)
{
    char request[294];
    user user = {0, "", "", "", NULL};
    strcat(strcat(strcpy(request, "SELECT name FROM user WHERE email = \""), username), "\"");
    strcpy(user.name, fetchColumn(request));
    strcat(strcat(strcpy(request, "SELECT id FROM user WHERE email = \""), username), "\"");
    user.id = atoi(fetchColumn(request));
    strcpy(user.email, username);
    strcpy(user.password, password);
    return user;
}

uint8_t verifyLogins(const char *username, const char *password)
{
    char request[298];

    strcpy(request, "SELECT password FROM user WHERE email = \"");
    strcat(request, username);
    strcat(request, "\"");

    MYSQL *conn = initBdd();
    char *passwordFetch = fetchColumn(request);
    if (passwordFetch == NULL)
    {
        return 0;
    }
    else
    {
        if (strcmp(passwordFetch, password) == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

uint8_t loginFileInvert(const char *loginFile)
{

    FILE *file = fopen(loginFile, "r+b");
    if (file != NULL)
    {
        uint64_t dataString[64] = {0};
        fread(dataString, 8, 64, file);
        for (uint8_t i = 0; i < 64; i++)
        {
            dataString[i] = ~dataString[i];
        }
        fseek(file, 0, SEEK_SET);
        fwrite(dataString, 8, 64, file);
        fclose(file);
        return 1;
    }
    exit(EXIT_FAILURE);
}

uint8_t verifyLoginFile(const char *loginFile)
{
    loginFileInvert(loginFile);
    FILE *file = fopen(loginFile, "rb");
    if (file != NULL)
    {
        char username[256];
        char password[256];
        fread(username, 256, 1, file);
        fread(password, 256, 1, file);
        if (verifyLogins(username, password))
        {
            loginFileInvert(loginFile);
            return 1;
        }
    }
    else
    {
        loginFileInvert(loginFile);
        return 2;
    }
}

uint8_t createLoginFile(const char *loginFile, const char *username, char *password)
{
    FILE *file = fopen(loginFile, "wb");
    if (file != NULL)
    {
        char username_[256] = "";
        char password_[256] = "";
        strcpy(username_, username);
        strcpy(password_, password);
        fwrite(username_, 256, 1, file);
        fwrite(password_, 256, 1, file);
        fclose(file);
        loginFileInvert(loginFile);
        return 1;
    }
    exit(EXIT_FAILURE);
}

char **getUserCred(const char *loginFile)
{
    loginFileInvert(loginFile);
    FILE *file = fopen(loginFile, "rb");
    char **userCred;

    if (file != NULL)
    {
        userCred = malloc(sizeof(char *) * 2);
        for (uint8_t i = 0; i < 2; i++)
        {
            userCred[i] = malloc(sizeof(char) * 256);
        }
        fread(userCred[0], 256, 1, file);
        fread(userCred[1], 256, 1, file);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    loginFileInvert(loginFile);
    return userCred;
}

uint8_t appDirectoryCheck(const char *appFolder)
{
    struct stat st = {0};
    if (stat(appFolder, &st) == -1)
    {
        if (mkdir(appFolder, 0700) != 0)
        {
            exit(EXIT_FAILURE);
        }
    }
    return 1;
}



void get_page(const char* url, const char* file_name){
    CURL *curl;
 
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        FILE* file = fopen(file_name, "w");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
 
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
 
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
 
        fclose(file);
    }
    curl_global_cleanup();
}

void freeSeriesNodeList(seriesNode **list)
{
    seriesNode *inter = *list;
    seriesNode *prev = NULL;
    while (inter != NULL)
    {
        prev = inter;
        inter = inter->next;
        free(prev);
    }
}

void freeSeasonsNodeList(seasonsNode **list)
{
    seasonsNode *inter = *list;
    seasonsNode *prev = NULL;
    while (inter != NULL)
    {
        prev = inter;
        inter = inter->next;
        free(prev);
    }
}

void freeEpisodesNodeList(episodesNode **list)
{
    episodesNode *inter = *list;
    episodesNode *prev = NULL;
    while (inter != NULL)
    {
        prev = inter;
        inter = inter->next;
        free(prev);
    }
}
uint16_t countSeriesList(episodesNode **series){
    episodesNode *inter = *series;
    uint16_t seriesNumber = 0;
    while (inter != NULL)
    {
        seriesNumber++;
        inter = inter->next;
    }
    return seriesNumber;
}

char * dayName(char * dayName){
    if (strcmp(dayName,"Monday") == 0){
        return "Lundi";
    }
    if (strcmp(dayName,"Tuesday") == 0){
        return "Mardi";
    }
    if (strcmp(dayName,"Wednesday") == 0){
        return "Mercredi";
    }
    if (strcmp(dayName,"Thursday") == 0){
        return "Jeudi";
    }
    if (strcmp(dayName,"Friday") == 0){
        return "Vendredi";
    }
    if (strcmp(dayName,"Saturday") == 0){
        return "Samedi";
    }
    if (strcmp(dayName,"Sunday") == 0){
        return "Dimanche";
    }
    return NULL;
}