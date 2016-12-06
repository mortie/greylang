CC=gcc
FLAGS=-g -Wall
FLAGS-VALGRIND=--quiet --leak-check=full --show-leak-kinds=all

TARGET = test

$(TARGET):
	$(CC) $(FLAGS) -o $(TARGET) *.c src/*.c

run: $(TARGET)
	valgrind $(FLAGS-VALGRIND) ./$(TARGET)
	rm -f $(TARGET)

clean:
	rm -f $(TARGET)
