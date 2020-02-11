#include <stdlib.h>
#include <gtk/gtk.h>
#include <mysql.h>
#include <stdint.h>
#include "shows.h"
genre *getSerieGenresList(int idSerie);
seriesNode *getUserSeriesList(int idUser);
void addUserSeries(user *user);
user createUserStruct(const char * username,const char * password);
uint8_t verifyLogins(const char * username, const char * password);
uint8_t loginFileInvert(const char *loginFile);
uint8_t verifyLoginFile(const char * loginFile);
char ** getUserCred(const char * loginFile);
uint8_t appDirectoryCheck(const char * appFolder);
uint8_t createLoginFile(const char *loginFile, const char * username, char * password);