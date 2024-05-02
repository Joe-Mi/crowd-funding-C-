# Makefile
# Created: 29/03/2024

# Variables
CXX = g++
CXXFLAGS = -std=c++17 -Wall
SOURCES = user.cpp fundraiser.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

# Default target
# all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleaning up
clean:
	-rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
