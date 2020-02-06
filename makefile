CC = gcc
BIN = projC
SRC = ./main.c \
	  ./database.c
OBJ = $(SRC:.c=.o)

ERROR_CFLAGS = -Wall -Werror -Wextra

GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`
MYSQL_CFLAGS = `mysql_config --cflags`
CFLAGS += $(ERROR_FLAGS) $(GTK_CFLAGS) $(MYSQL_CFLAGS) -I./

MYSQL_LDFLAGS = `mysql_config --libs`
GTK_LDFLAGS = `pkg-config --libs gtk+-3.0`
LDFLAGS +=  $(GTK_LDFLAGS) $(MYSQL_LDFLAGS) -I./

all: $(BIN)

$(BIN): database.o main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#
#$(OBJ): $(SRC)
#	$(CC) $(CFLAGS) -o $@ -c $<

database.o: database.c
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re