#include <stdlib.h>
#include <gtk/gtk.h>
#include <mysql.h>
#include <stdint.h>
#include "database.h"
#include "manageGtk.h"
#include "shows.h"

uint8_t verifyLogins(char * username,char * password);