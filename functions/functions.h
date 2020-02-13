#include <stdint.h>
#include "shows.h"
episode getEpisode(int idEpisode);
episodesNode * getSerieSeasonEpisodes(int idSerie, int seasonNum);
seasonsNode * getSerieSeasonsList(int idSerie);
serie getSerieStruct(int idSerie);
seriesNode *getUserSeriesList(int idUser);
void addUserSeries(user *user);
user createUserStruct(const char * username,const char * password);
uint8_t verifyLogins(const char * username, const char * password);
uint8_t loginFileInvert(const char *loginFile);
uint8_t verifyLoginFile(const char * loginFile);
char ** getUserCred(const char * loginFile);
uint8_t appDirectoryCheck(const char * appFolder);
uint8_t createLoginFile(const char *loginFile, const char * username, char * password);
void freeSeriesNodeList(seriesNode **list);
void freeSeasonsNodeList(seasonsNode **list);
void freeEpisodesNodeList(episodesNode **list);
void get_page(const char* url, const char* file_name);