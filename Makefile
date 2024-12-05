CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = data.o knn.o

all: knn

data.o: data.c headers.h
	$(CC) $(CFLAGS) -c data.c

knn.o: knn.c headers.h
	$(CC) $(CFLAGS) -c knn.c

knn: $(OBJ)
	$(CC) $(CFLAGS) -o knn $(OBJ)

clean:
	rm -f *.o knn
