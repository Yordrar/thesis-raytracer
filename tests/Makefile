CSOURCES = $(wildcard */*.cpp) $(wildcard ../src/math/*.cpp)

OBJECTS = $(patsubst %.cpp, %.o, $(CSOURCES))

GTEST = C:/GTest

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
	rm -rf $(OBJECTS) $(TARGET)