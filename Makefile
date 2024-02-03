CFLAGS  = -O2 -Wall -Wno-missing-braces -I../include/
LDFLAGS = -L../lib/
LDLIBS  = -lraylib -lopengl32 -lgdi32 -lwinmm
OBJ_DIR = object_files
COMPILER = g++
TARGET  = Build.exe

# List of source files
SRCS = $(wildcard src/*.cpp)
# Create a list of object files from source files
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:src/%.cpp=%.o))

# Default target
all: $(TARGET)

# Create the object_files directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

# Rule to build object files
$(OBJ_DIR)/%.o: src/%.cpp
	$(COMPILER) $(CFLAGS) -c $< -o $@

# Rule to build the executable
$(TARGET): $(OBJS)
	$(COMPILER) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

# Clean rule
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)