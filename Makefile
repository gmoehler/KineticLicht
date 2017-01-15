CC=g++
INC_DIR = /c/Users/gregor/Documents/GitHub/googletest/googletest/include
CFLAGS=-c -Wall -I$(INC_DIR)
LDFLAGS=-L. libgtest_main.a  libgtest.a
SOURCES=src/RGB.cpp src/test/test_main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
