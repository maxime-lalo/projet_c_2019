#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <stdint.h>
#include "database.h"
#include "functions.h"

uint8_t verifyLogins(char * username,char * password){
    char request[255];
    strcpy(request,"SELECT password FROM user WHERE email = \"");
    strcat(request,username);
    strcat(request,"\"");

    MYSQL * conn = initBdd();
    char * passwordFetch = fetchColumn(request);
    if(passwordFetch == NULL){
        return 0;
    }else{
        if(strcmp(passwordFetch,password) == 0){
           return 1; 
        }else{
            return 0;
        }
    }
}

uint8_t isConnected(){
    // Vérifier si l'utilisateur est connecté
    return 0;
}