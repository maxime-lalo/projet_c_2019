CC = gcc
BIN = projC
SRC = ./main.c \
	  ./functions/database.c \
	  ./functions/manageGtk.c \
	  ./functions/shows.c
OBJ = $(SRC:.c=.o)

ERROR_CFLAGS = -Wall -Werror -Wextra

GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`
MYSQL_CFLAGS = `mysql_config --cflags`
CURL_CFLAGS = `curl-config --cflags`
CFLAGS += $(ERROR_FLAGS) $(GTK_CFLAGS) $(MYSQL_CFLAGS) $(CURL_CFLAGS) -I./

MYSQL_LDFLAGS = `mysql_config --libs`
GTK_LDFLAGS = `pkg-config --libs gtk+-3.0`
CURL_LDFLAGS = `curl-config --libs`
LDFLAGS +=  $(GTK_LDFLAGS) $(MYSQL_LDFLAGS) $(CURL_LDFLAGS) -I./

all: $(BIN)

$(BIN): database.o main.o manageGtk.o shows.o functions.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#
#$(OBJ): $(SRC)
#	$(CC) $(CFLAGS) -o $@ -c $<

database.o: ./functions/database.c
	$(CC) $(CFLAGS) -o $@ -c $<

manageGtk.o : ./functions/manageGtk.c
	$(CC) $(CFLAGS) -o $@ -c $<

shows.o : ./functions/shows.c
	$(CC) $(CFLAGS) -o $@ -c $<

functions.o : ./functions/functions.c
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -f $(OBJ)

run: fclean all
	./$(BIN)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re