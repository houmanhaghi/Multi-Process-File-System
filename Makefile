# Makefile

# Define the compiler and compiler flags
CC := gcc
CFLAGS := -Wall -Wextra -g -pthread

# Define the build target
TARGET := fisys

# Define the directories you want to create
ROOT := root
# Define the subdirectories for root
SUBDIRS := root/bin root/dev root/etc root/home root/mnt root/proc root/tmp root/usr

# Define the default target (what happens when you just run `make` without any specific target)
all: create_dirs create_meta_files $(TARGET)

# Rule to build the C code
$(TARGET): src/main.c
	$(CC) $(CFLAGS) $< -o $@
	

# Rule to create directories (including subdirectories)
create_dirs:
	mkdir -p $(ROOT) $(SUBDIRS)

# Rule to create .meta files in each directory
create_meta_files:
	echo "" > $(ROOT)/.meta
	echo "" > $(ROOT)/bin/.meta
	echo "" > $(ROOT)/dev/.meta
	echo "" > $(ROOT)/etc/.meta
	echo "" > $(ROOT)/home/.meta
	echo "" > $(ROOT)/mnt/.meta
	echo "" > $(ROOT)/proc/.meta
	echo "" > $(ROOT)/tmp/.meta
	echo "" > $(ROOT)/usr/.meta

# need admin access to run binary file
run:
	sudo ./$(TARGET)

# Rule to remove directories and the binary
clean:
	rm -rf $(ROOT) $(TARGET)
