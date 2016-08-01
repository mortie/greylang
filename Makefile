CC = gcc
CFLAGS = -Wall -lm -lreadline -ldl
CFLAGS-BLD = -O3
CFLAGS-DBG = -g -DDEBUG
PREFIX = /usr/local/bin

TARGET = grey

build:
	$(CC) $(CFLAGS) $(CFLAGS-BLD) -o $(TARGET) $(shell find src -name '*.c')

debug:
	$(CC) $(CFLAGS) $(CFLAGS-DBG) -o $(TARGET) $(shell find src -name '*.c')

install:
	mv $(TARGET) $(PREFIX)/$(TARGET)

clean:
	rm $(TARGET)
