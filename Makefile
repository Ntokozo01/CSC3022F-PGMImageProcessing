# Makefile for building driver.exe from factorial.o and driver.o

# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++20

# Target executable
TARGET = driver.exe
TEST_TARGET = drivertest.exe

# Source files
SRCS = ConnectedComponent.cpp PGMimageProcessor.cpp driver.cpp ConnectedComponent.h PGMimageProcessor.h
TEST_SRCS = ConnectedComponent.cpp PGMimageProcessor.cpp drivertest.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# specialised target
test: $(TEST_TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $(TEST_TARGET) $(CFLAGS)

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
	rm *.o *.exe
#	find . -type f -name '*.pgm' ! \( -name 'chess.pgm' -o -name 'cheetah.pgm' -o -name 'chess-thresh-separate.pgm' -o -name 'boundary.pgm' -o -name 'House.pgm' -o -name 'maize.pgm' -o -name 'camera-man.pgm' -o -name 'shapes.pgm' \) -exec rm {} +