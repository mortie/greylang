CC = gcc
CFLAGS = -Wall -lm -lreadline -ldl
CFLAGS-BLD = -O3
CFLAGS-DBG = -g -DDEBUG
PREFIX = /usr/local/bin

TARGET = grey

build: preprocess
	$(CC) -o $(TARGET) $(shell find src -name '*.c') $(CFLAGS) $(CFLAGS-BLD)

debug: preprocess
	$(CC) -o $(TARGET) $(shell find src -name '*.c') $(CFLAGS) $(CFLAGS-DBG)

preprocess:
	$(shell sh preprocess.sh)

install:
	mv $(TARGET) $(PREFIX)/$(TARGET)

clean:
	rm $(TARGET)
	rm "stdlib.out"
