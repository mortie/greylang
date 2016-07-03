CC = gcc
CFLAGS = -g -Wall -lm -lreadline

TARGET = grey

build:
	$(CC) $(CFLAGS) -o $(TARGET) $(shell find src -name '*.c')

install:
	mv $(TARGET) /usr/local/bin/$(TARGET)
