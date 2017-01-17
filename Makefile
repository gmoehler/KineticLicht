#Compiler and Linker
CC          := g++

#The Target Binary Program
TARGET      := dotest

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := src
TESTDIR     := test
TESTINCDIR  := test
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
OBJEXT      := obj

#Flags, Libraries and Includes
CFLAGS      := -fopenmp -Wall -O3 -g -std=gnu++11 -DWITHIN_UNITTEST
LIB         := -fopenmp -lm -L. -lgtest_main -lgtest
GOOGLETEST_DIR := /c/Users/gregor/Documents/GitHub/googletest/googletest
INC         := -I$(INCDIR) -I$(TESTINCDIR) -I/usr/local/include -I$(GOOGLETEST_DIR)/include
INCDEP      := -I$(INCDIR) -I$(TESTINCDIR) -I$(GOOGLETEST_DIR)/include

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
#SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES      := src/RGB.cpp src/Animation.cpp src/AnimationStore.cpp
TESTSOURCES  := test/test_rgb.cpp test/test_KeyFrame.cpp test/test_Animation.cpp test/test_AnimationStore.cpp

OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT))) \
							 $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: $(TARGET)

#Remake
remake: cleaner all

#Copy Resources from Resources Directory to Target Directory
resources: directories
	@cp $(RESDIR)/* $(TARGETDIR)/

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(BUILDDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/%.$(OBJEXT): $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

#Non-File Targets
.PHONY: all remake clean cleaner resources
