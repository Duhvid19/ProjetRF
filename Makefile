CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I. -Itools
SRC = main.c data.c knn.c kmeans.c test.c
OBJ = $(SRC:.c=.o)
TARGET = project

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
