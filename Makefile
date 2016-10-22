CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wpedantic -lm -lreadline -ldl
OFLAGS = -O3
VFLAGS = --track-origins=yes
FLAGS-DBG = -g -DDEBUG
PREFIX = /usr/local/bin

PROJECT = grey

CDIR = src
ODIR = obj

# Find all .C files, and thus the corresponding .o files
CFILES := $(shell find $(CDIR) -type f -name '*.c')
OFILES := $(patsubst $(CDIR)/%.c,$(ODIR)/%.o,$(CFILES))

# Compile the project
$(PROJECT): stdlib.out $(OFILES)
	$(CC) -o $(PROJECT) $(CFLAGS) $(OFILES)

# Compile without optimization and with debug info
debug: OFLAGS = $(FLAGS-DBG)
debug: CFLAGS += $(FLAGS-DBG)
debug: $(PROJECT)

# Compile and run
run: $(PROJECT)
	$(PROJECT)

# Compile with debug info, run with valgrind
test: debug
	valgrind $(VFLAGS) ./$(PROJECT)

# Install to $(PREFIX)
install: $(PROJECT)
	mv $(PROJECT) $(PREFIX)

# Uninstall
uninstall:
	rm $(PREFIX)/$(PROJECT)

# Clean the project
clean:
	rm -f -r $(ODIR)
	rm -f $(PROJECT)
	rm -f stdlib.out
	rm -f vgcore.*

# Compile .c files to .o files
$(ODIR)/%.o: $(CDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $(patsubst $(ODIR)/%.o,$(CDIR)/%.c,$@) -o $@ $(OFLAGS)

# Run stdlib preprocessor thing
stdlib.out: $(shell find stdlib -type f)
	./preprocess.sh
