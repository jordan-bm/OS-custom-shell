# Compiler settings
CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Wextra
RUID = jjb420
TARGET = shell

# Source file
SRC = $(RUID)_assignment1.c

# Build the shell
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Remove binary
clean:
	rm -f $(TARGET)

# Run the shell
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run