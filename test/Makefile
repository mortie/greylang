CC = gcc

TARGET = test

$(TARGET):
	$(CC) -o $(TARGET) *.c src/*.c

run: $(TARGET)
	valgrind ./$(TARGET)
	rm -f $(TARGET)

clean:
	rm -f $(TARGET)
