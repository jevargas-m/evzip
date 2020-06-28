# Define variable CC to be the compiler we want to use
CC = clang

# Define CFLAGS for the flags we will want to use with compiler
CFLAGS = -g -Wall

# Output executable
TARGET = testhuff

# Output object files dependencies for linking
TARGET_DEPENDENCIES = testhuff.o huffmantree.o heap.o

# ensure these targets are executed even if there is a file named like that
.PHONY: clean all

# If no arguments are passed to make, it will attempt the 'all' target
default: all
all: $(TARGET) 

# TARGET_DEPENDENCIES
# $< first dependency,  $^ all dependencies,   $@ the file LHS of ':'

testhuff.o: testhuff.c ../huffmantree.h 
	$(CC) -c $(CFLAGS) $< -o $@

huffmantree.o: ../huffmantree.c ../huffmantree.h 
	$(CC) -c $(CFLAGS) $< -o $@

heap.o: ../heap/heap.c ../heap/heap.h
	$(CC) -c $(CFLAGS) $< -o $@

# TARGET linking
$(TARGET): $(TARGET_DEPENDENCIES)
	$(CC) $(CFLAGS) $^ -o $@

# $(RM) is the platform agnostic way to delete a file
clean:
	$(RM) $(TARGET)
	$(RM) $(TARGET_DEPENDENCIES)
