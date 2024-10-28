# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -g

# Libraries
LIBS = 

# Output binary
OUTPUT = output/http

# Source files
SRCS = main.cc src/server.cc

# Object files (replace .cc with output/.o)
OBJS = $(SRCS:%.cc=output/%.o)

# Default target
all: $(OUTPUT)

# Link the object files to create the final executable
$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUTPUT) $(LIBS)

# Compile source files into object files in the output/ directory
output/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf output/*.o $(OUTPUT)
