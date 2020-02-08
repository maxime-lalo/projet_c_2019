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

uint8_t verifyLoginFile(const char *loginFile)
{
    FILE *file = fopen(loginFile, "r+b");
    if (file != NULL)
    {
        char username[256];
        char password[256];
        fread(username,256,1,file);
        fread(password,256,1,file);
        if(verifyLogins(username, password)){
            return 1;
        }
    }else
    {
        return 2;
        
    }
}

uint8_t createLoginFile(const char *loginFile, const char * username, char * password){
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
        return 1;
    }
    exit(EXIT_FAILURE);
}

uint8_t appDirectoryCheck(const char * appFolder)
{
    struct stat st = {0};
    if (stat(appFolder, &st) == -1)
    {
        if (mkdir(appFolder, 0700)!= 0)
        {
            exit(EXIT_FAILURE);
        }
    }
    return 1;
}