CC=gcc
FLAGS=-g -Wall -lm -lreadline -ldl
FLAGS-VALGRIND=--quiet --leak-check=full --show-leak-kinds=all

TARGET = test

$(TARGET):
	$(CC) $(FLAGS) -o $(TARGET) *.c src/*.c $(shell find ../obj/*/* -type f)

run: $(TARGET)
	valgrind $(FLAGS-VALGRIND) ./$(TARGET)
	rm -f $(TARGET)

clean:
	rm -f $(TARGET)
