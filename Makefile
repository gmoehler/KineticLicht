CC=g++

GOOGLETEST_DIR=/c/Users/gregor/Documents/GitHub/googletest/googletest
INC_DIR = $(GOOGLETEST_DIR)/include

CFLAGS=-c -Wall -I$(INC_DIR) -DTEST
LDFLAGS=-L. libgtest_main.a  libgtest.a

SOURCES=src/RGB.cpp
TEST_SOURCES=test/test_rgb.cpp test/test_KeyFrame.cpp

OBJECTS=$(SOURCES:.cpp=.o) $(TEST_SOURCES:.cpp=.o)
EXECUTABLE=dotest


all: $(SOURCES) $(TEST_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
