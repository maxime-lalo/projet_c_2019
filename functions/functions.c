#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "database.h"
#include "functions.h"

uint8_t verifyLogins(const char *username,const char *password)
{
    char request[255];
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

uint8_t verifyLoginFile(const char *loginFile)
{
    FILE *file = fopen(loginFile, "r+");
    if (file != NULL)
    {
        printf("test");
    }
    return 0;
}

uint8_t appDirectoryCheck(const char * appFolder)
{
    struct stat st = {0};
    if (stat(appFolder, &st) == -1)
    {
        if (mkdir(appFolder, 0700) == 0)
        {
            printf("Directory is OK");
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }
    return 1;
}

uint8_t isConnected(){
    // Vérifier si l'utilisateur est connecté
    return 0;
}