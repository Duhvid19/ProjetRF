CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I. -Itools
SRC = main.c data.c knn.c kmeans.c tools/evaluation.c
OBJ = $(SRC:.c=.o)
TARGET = test_project

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
