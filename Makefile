CC = gcc
#OPT = -Wall -Wextra -O2
OPT = -Wall -Wextra -O2 -DALPHA -DBETA
SRC =	awari.c
PROG = awari

$(PROG) : $(SRC)
	$(CC) $(OPT) -o $(PROG) $(SRC)

clean:
	rm $(PROG)
