#Compiler and Linker
CC          := g++

#The Target Binary Program
TARGET      := dotest

# what platform we are running on
PLATFORM    := $(shell uname -so | sed -r s'/[^a-zA-Z0-9]/_/g')

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := KineticLicht
INCDIR      := KineticLicht
TESTDIR     := test
TESTINCDIR  := test
BUILDDIR    := obj
TARGETDIR   := bin
RESDIR      := res
SRCEXT      := cpp
OBJEXT      := obj
LIBDIR      := test/lib/$(PLATFORM)


#Flags, Libraries and Includes
CFLAGS      := -ggdb -static-libgcc -static-libstdc++ -fopenmp -Wall -O3 -g -std=gnu++11 -DWITHIN_UNITTEST
LIB         := -fopenmp -lm -L$(LIBDIR) -lgtest_main -lgtest
GOOGLETEST_DIR := /c/Users/gregor/Documents/GitHub/googletest/googletest
INC         := -I$(INCDIR) -I$(TESTINCDIR) -Itest/include -I/usr/local/include -I$(GOOGLETEST_DIR)/include
INCDEP      := -I$(INCDIR) -I$(TESTINCDIR) -I$(GOOGLETEST_DIR)/include

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
#SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES      := $(SRCDIR)/RGB.cpp $(SRCDIR)/Animation.cpp $(SRCDIR)/AnimationOps.cpp \
								$(SRCDIR)/LedWorker.cpp $(SRCDIR)/StepperWorker.cpp $(SRCDIR)/AnimationList.cpp
TESTSOURCES  := $(TESTDIR)/mock_Arduino.cpp  $(TESTDIR)/test_rgb.cpp $(TESTDIR)/test_KeyFrame.cpp \
					$(TESTDIR)/test_Animation.cpp  $(TESTDIR)/test_AnimationOps.cpp \
					$(TESTDIR)/test_LedWorker.cpp $(TESTDIR)/test_StepperWorker.cpp \
					$(TESTDIR)/test_FiniteStates.cpp

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
