#ifndef __database_h
#define __database_h

#include <mysql.h>
#include <stdio.h>

//Episodes
typedef struct episode
{
    int id;
    int idSerie;
    int season;
    int number;
    char name[256];
    char shortDescription[1000];
    char airDate[11];
    int duration;
    struct episode *next;
} episode;

//Seasons
typedef struct season
{
    int number;
    episode *episodes;
} season;

typedef struct seasonNodes
{
    season *season;
    struct seasonNodes *next;
} seasonNodes;

//Genres
typedef struct genre
{
    int id;
    char name[256];
    struct genre *next;
} genre;

//Series
typedef struct serie
{
    int id;
    char name[256];
    char ***genre;
    char imageLink[1000];
    int state;
    char genre[256];
    int seasonNumber;
    seasonNodes *seasonNodes;
} serie;

typedef struct seriesNode
{
    serie *serie;
    struct seriesNode *next;
} seriesNode;

//User
typedef struct user
{
    int id;
    char name[256];
    char password[256];
    char email[256];
    seriesNode *series;
} user;

MYSQL *initBdd();
MYSQL_ROW fetchRow(char *request);
void fetchAllRows(char *request, MYSQL_RES **res);
char *fetchColumn(char *request);

#endif // __database_hls