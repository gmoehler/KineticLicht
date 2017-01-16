CC=g++

GOOGLETEST_DIR=/c/Users/gregor/Documents/GitHub/googletest/googletest
INC_DIR = $(GOOGLETEST_DIR)/include
ODIR=obj

CFLAGS=-c -Wall -I$(INC_DIR) -DTEST -std=gnu++11
ARDUINO_CFLAGS=-fno-exceptions -fno-threadsafe-statics -fpermissive -std=gnu++11 -g -Os -Wall -ffunction-sections -fdata-sections -flto
LDFLAGS=-L. libgtest_main.a  libgtest.a

SOURCES=src/RGB.cpp src/Animation.cpp
TEST_SOURCES=test/test_rgb.cpp test/test_KeyFrame.cpp test/test_Animation.cpp

OBJECTS=$(SOURCES:.cpp=.o) $(TEST_SOURCES:.cpp=.o)
EXECUTABLE=dotest


all: $(SOURCES) $(TEST_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
		-rm -f src/*.o test/*.o
