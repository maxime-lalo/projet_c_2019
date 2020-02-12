#include <stdlib.h>
#include <gtk/gtk.h>
#include <mysql.h>
#include <stdint.h>
#include "shows.h"
genre *getSerieGenresList(int idSerie);
season *getSeasonEpisodesStruct(int seasonNum, int idSerie);
seasonNodes *getSerieSeasonsList(int idSerie);
serie *getSerieStruct(int idSerie);
seriesNode *getUserSeriesList(int idUser);
void addUserSeries(user *user);
user createUserStruct(const char * username,const char * password);
uint8_t verifyLogins(const char * username, const char * password);
uint8_t loginFileInvert(const char *loginFile);
uint8_t verifyLoginFile(const char * loginFile);
char ** getUserCred(const char * loginFile);
uint8_t appDirectoryCheck(const char * appFolder);
uint8_t createLoginFile(const char *loginFile, const char * username, char * password);
void freeEpisodeList(episode **list);
void freeSeasonStruct(season **seasonStruct);
void freeSeasonNodesList(seasonNodes **list);
void freeGenreList(genre **list);
void freeSerieStruct(serie **serieStruct);
void freeSeriesNodeList(seriesNode **list);