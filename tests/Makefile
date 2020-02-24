TESTS = $(wildcard */*.cpp)
CPPSOURCES = $(wildcard ../src/math/*.cpp)

OBJECTS = $(patsubst %.cpp, %.o, $(CPPSOURCES)) $(patsubst %.cpp, %.o, $(TESTS))

GTEST = ../deps/gtest

CC = g++
CFLAGS = -std=gnu++11 -I$(GTEST)/include -I../src -c

LINKFLAGS = -L$(GTEST)/lib
LIBRARIES = -static -lgtest -lgtest_main

TARGET = all_tests

.PHONY: run clean

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LINKFLAGS) $(OBJECTS) $(LIBRARIES) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(patsubst %.cpp, %.o, $(TESTS)) $(TARGET)