# Makefile for building driver.exe from factorial.o and driver.o

# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++20

# Target executable
TARGET = driver.exe

# Source files
SRCS = ConnectedComponent.cpp PGMimageProcessor.cpp driver.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# Run the target file
# make run options="-s 4 -n 15 -i mona.pgm" inputFileImage="mona512.pgm" 
run: $(TARGET)
	./$(TARGET) $(options) $(inputFileImage)

.PHONY: clean

# Clean target to clean all compiled object files and executable file and remove generated PGM images 
clean:
	rm -f $(OBJS) $(TARGET)
#	find . -type f -name '*.pgm' ! \( -name 'chess.pgm' -o -name 'cheetah.pgm' -o -name 'chess-thresh-separate.pgm' \) -exec rm {} +