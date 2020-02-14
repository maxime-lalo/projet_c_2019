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
    int airDay;
    int airMonth;
    int airYear;
    int duration;
} episode;

typedef struct episodesNode
{
    episode episode;
    struct episodesNode *next;
} episodesNode;

//Seasons
typedef struct seasonsNode
{
    int number;
    episodesNode *episodes;
    struct seasonsNode *next;
} seasonsNode;

//Genres
typedef struct genre
{
    int id;
    char name[256];
} genre;

typedef struct genresNode
{
    genre genre;
    struct genresNode *next;
} genresNode;

//Series
typedef struct serie
{
    int id;
    char name[256];
    char imageLink[255];
    int state;
} serie;

typedef struct seriesNode
{
    serie serie;
    genre *genre;
    seasonsNode *seasons;
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
char * dayName(char * dayName);
u_int8_t sqlExecute(char * request);
#endif // __database_hls