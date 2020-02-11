#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "database.h"
#include "functions.h"

genre *getSerieGenresList(int idSerie)
{
    char request[294];
    char idSerieString[5];
    MYSQL_ROW rowGenres;
    MYSQL_ROW rowGenre;
    MYSQL_RES *resultSerieGenres;
    MYSQL_RES *resultGenre;
    genre *genreInter;
    genre *genreStart;

    sprintf(idSerieString, "%d", idSerie);
    strcat(strcat(strcpy(request, "SELECT * FROM serie_genre WHERE id_serie = \""), idSerieString), "\"");
    fetchAllRows(request, &resultSerieGenres);

    while ((rowGenres = mysql_fetch_row(resultSerieGenres)) != NULL)
    {
        strcat(strcat(strcpy(request, "SELECT * FROM genre WHERE id = \""), rowGenres[0]), "\"");
        fetchAllRows(request, &resultGenre);

        while ((rowGenre = mysql_fetch_row(resultGenre)) != NULL)
        {
            genreInter = malloc(sizeof(genre));
            genreInter->id = atoi(rowGenre[0]);
            strcpy(genreInter->name, rowGenre[1]);
            genreInter->next = genreStart;
            genreStart->next = genreInter;
        }
    }
    return genreStart;
}



seasonNodes *getSerieSeasonsList(int idSerie)
{
    uint8_t seasonNum = 1;
    char request[294];
    char idSerieString[5];
    char seasonNumString[5];
    MYSQL_ROW rowSeasons;
    MYSQL_ROW rowSeason;
    MYSQL_RES *resultSerieSeasons;
    MYSQL_RES *resultSeason;
    seasonNodes *nodeInter;
    seasonNodes *nodeStart;
    serie *serieInter;

    sprintf(idSerieString, "%d", idSerie);
    sprintf(seasonNumString, "%d", seasonNum);

    strcat(strcat(strcat(strcat(strcpy(request, "SELECT * FROM episode WHERE id_serie =\""), idSerieString), "\" AND season =\""), seasonNumString), "\"");

    fetchAllRows(request, &resultSerieSeasons);

    while ((rowSeasons = mysql_fetch_row(resultSerieSeasons)) != NULL)
    {
        /*
        genreInter = malloc(sizeof(genre));
        genreInter->id = atoi(row[0]);
        strcpy(genreInter->name, row[1]);
        genreInter->next = genreStart;
        genreStart->next = genreInter;
        seasonNum++;
        sprintf(seasonNumString, "%d", seasonNum);
        strcat(strcat(strcat(strcat(strcpy(request, "SELECT * FROM episode WHERE id_serie =\""), idSerieString), "\" AND season =\""), seasonNumString), "\"");
        */
    }
    return nodeStart;
}

seriesNode *getUserSeriesList(int idUser)
{
    char request[294];
    char idUserString[5];
    MYSQL_ROW rowSeries;
    MYSQL_ROW rowSerie;
    MYSQL_RES *resultUserSeries;
    MYSQL_RES *resultSerie;
    seriesNode *nodeStart;
    seriesNode *nodeInter;
    serie *serieInter;
    sprintf(idUserString, "%d", idUser);
    strcat(strcat(strcpy(request, "SELECT * FROM serie_user WHERE id_user = \""), idUserString), "\"");
    fetchAllRows(request, &resultUserSeries);
    while ((rowSeries = mysql_fetch_row(resultUserSeries)) != NULL)
    {
        strcat(strcat(strcpy(request, "SELECT * FROM serie WHERE id = \""), rowSeries[0]), "\"");
        fetchAllRows(request, &resultSerie);
        while ((rowSerie = mysql_fetch_row(resultSerie)) != NULL)
        {
            serieInter = malloc(sizeof(serie));
            nodeInter = malloc(sizeof(seriesNode));
            serieInter->id = atoi(rowSerie[0]);
            strcpy(serieInter->name, rowSerie[1]);
            strcpy(serieInter->imageLink, rowSerie[2]);
            serieInter->state = atoi(rowSerie[3]);
            serieInter->genre = getSerieGenresList(serieInter->id);
            //
            nodeInter->serie = serieInter;
            nodeInter->next = nodeStart;
            nodeStart->next = nodeInter;
        }
    }
    return nodeStart;
}

void addUserSeries(user *user)
{
    char request[294];
    char userId[5];
    int numFields;

    MYSQL_RES *resultSeriesGenres;
    MYSQL_ROW row;
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