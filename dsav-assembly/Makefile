# ARMv8 Assembly DSAV - Data Structures & Algorithms Visualizer
# Build System using m4 preprocessor and gcc

CC = gcarm 						# I have my own binary for executing arm assembly files in an x86 environment, if you have an arm chip use gcc.
M4 = m4
CFLAGS = -g -no-pie
SOURCES = main.asm ansi.asm display.asm array_viz.asm stack_viz.asm \
          queue_viz.asm linkedlist_viz.asm sort_viz.asm search_viz.asm utils.asm
PREPROCESSED = $(SOURCES:.asm=.s)
OBJECTS = $(SOURCES:.asm=.o)
TARGET = dsav

.PHONY: all clean test

all: $(TARGET)

# Preprocess .asm files with m4 to expand macros
%.s: %.asm macros.m4
	$(M4) $< > $@

# Assemble .s files to object files
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

# Link all object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)
	@echo "Run with: ./$(TARGET)"

# Clean build artifacts
clean:
	rm -f *.s *.o $(TARGET)
	@echo "Clean complete"

# Test: build and run
test: $(TARGET)
	./$(TARGET)
