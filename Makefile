CC = gcc
CFLAGS = -g -Wall -lm

TARGET = lang

build:
	$(CC) $(CFLAGS) -o $(TARGET) $(shell find src -name '*.c')
