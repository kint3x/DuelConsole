CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./
CXXFLAGS += $(shell pkg-config --cflags sdl2)
LDFLAGS  := $(shell pkg-config --libs sdl2)

SRC := $(shell find . -name "*.cpp")

# Map src files to bin/*.o
OBJ := $(patsubst %.cpp, bin/%.o, $(SRC))

TARGET := bin/game

.PHONY: all clean run

all: $(TARGET)

build_resources:
	python3 build_fonts.py
	python3 build_resources.py

# Link
$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile (this is the important part)
bin/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf generated/
	rm -rf bin/*

run: $(TARGET)
	./$(TARGET)