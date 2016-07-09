CC = gcc
CFLAGS = -Wall -lm -lreadline -ldl
CFLAGS-BLD = -O3
CFLAGS-DBG = -g -DDEBUG

TARGET = grey

build:
	$(CC) $(CFLAGS) $(CFLAGS-BLD) -o $(TARGET) $(shell find src -name '*.c')

debug:
	$(CC) $(CFLAGS) $(CFLAGS-DBG) -o $(TARGET) $(shell find src -name '*.c')

install:
	mv $(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	rm /usr/local/bin/$(TARGET)
