# Define variable CC to be the compiler we want to use
CC = clang

# Define CFLAGS for the flags we will want to use with clang
CFLAGS = -g -Wall -pthread

# Output executable
TARGET = testheap

# Output object files dependencies for linking
TARGET_DEPENDENCIES = heap.o testheap.o

# ensure these targets are executed even if there is a file named like that
.PHONY: clean all

# If no arguments are passed to make, it will attempt the 'all' target
default: all
all: $(TARGET) 

# TARGET_DEPENDENCIES
# $< first dependency,  $^ all dependencies,   $@ the file LHS of ':'

testheap.o: testheap.c 
	$(CC) -c $(CFLAGS) $< -o $@

heap.o: ../heap.c ../heap.h 
	$(CC) -c $(CFLAGS) $< -o $@

# TARGET linking
$(TARGET): $(TARGET_DEPENDENCIES)
	$(CC) $(CFLAGS) $^ -o $@

# $(RM) is the platform agnostic way to delete a file (here rm -f)
clean:
	$(RM) $(TARGET)
	$(RM) $(TARGET_DEPENDENCIES)
