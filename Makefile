# Define variable CC to be the compiler we want to use
CC = gcc

# Define CFLAGS for the flags we will want to use with compiler
CFLAGS = -g -Wall

# Output executable
TARGET1 = evzip
TARGET2 = evunzip

# Output object files dependencies for linking
TARGET1_DEPENDENCIES = evzip.o huffmantree.o heap.o
TARGET2_DEPENDENCIES = evunzip.o huffmantree.o heap.o

# ensure these targets are executed even if there is a file named like that
.PHONY: clean all

# If no arguments are passed to make, it will attempt the 'all' target
default: all
all: $(TARGET1) $(TARGET2) 

# TARGET_DEPENDENCIES
# $< first dependency,  $^ all dependencies,   $@ the file LHS of ':'

evzip.o: evzip.c huffmantree.h 
	$(CC) -c $(CFLAGS) $< -o $@

evunzip.o: evunzip.c huffmantree.h 
	$(CC) -c $(CFLAGS) $< -o $@

huffmantree.o: huffmantree.c huffmantree.h 
	$(CC) -c $(CFLAGS) $< -o $@

heap.o: heap/heap.c heap/heap.h
	$(CC) -c $(CFLAGS) $< -o $@

# TARGET linking
$(TARGET1): $(TARGET1_DEPENDENCIES)
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET2): $(TARGET2_DEPENDENCIES)
	$(CC) $(CFLAGS) $^ -o $@

# $(RM) is the platform agnostic way to delete a file
clean:
	$(RM) $(TARGET1)
	$(RM) $(TARGET1_DEPENDENCIES)
	$(RM) $(TARGET2)
	$(RM) $(TARGET2_DEPENDENCIES)
