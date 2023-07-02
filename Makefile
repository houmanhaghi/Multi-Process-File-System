# Makefile

# Define the compiler and compiler flags
CC := gcc
CFLAGS := -Wall -Wextra -g -pthread

# Define the build target
TARGET := FileSys
MAINC := src/main.c

# Define the directories you want to create
ROOT := root
# Define the subdirectories for root
SUBDIRS := root/bin root/dev root/etc root/home root/mnt root/proc root/tmp root/usr

# Define the default target (what happens when you just run `make` without any specific target)
all: $(TARGET) create_dirs

# Rule to build the C code
$(TARGET): $(MAINC)
	$(CC) $(CFLAGS) -o $@ $<

# Rule to create directories (including subdirectories)
create_dirs:
	mkdir -p $(ROOT) $(SUBDIRS)

# Rule to remove directories and the binary
clean:
	rm -rf $(ROOT) $(TARGET)
