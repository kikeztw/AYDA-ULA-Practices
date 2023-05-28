CXX         = clang++ -std=c++17
WARN        = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings -Wno-parentheses -Wfloat-equal -pedantic

INCLUDEPATH = -I$(LIB)/include

# Comment the following to lines (add a # at the beginning of the line) if you want to compile in graphice mode
FLAGS       = -DDEBUG -g -O0
LIBLINK     = $(OBJECTS) -lpthread 

# Unomment the following to lines (remove the # from the beginning of the line) if you want to compile in graphice mode
# FLAGS       = -DRENDER_MODE_GRAPHIC -Ofast
# LIBLINK    = $(OBJECTS) -lpthread -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

# Libs
LIB         = ./lib
INCLUDES    = $(wildcard $(LIB)/include/*.hpp)
SOURCES     = $(wildcard $(LIB)/src/*.cpp)
OBJECTS     = $(SOURCES:$(LIB)/src/%.cpp=$(LIB)/obj/%.o)

# Test
#TESTDIR     = ./test
#TEST        = $(wildcard $(TESTDIR)/src/*.cpp)
#MKTEST      = $(TEST:$(TESTDIR)/src/%.cpp=$(TESTDIR)/bin/%)

# Main
MAINDIR     = ./main
MAIN        = $(wildcard $(MAINDIR)/src/*.cpp)
MKMAIN      = $(MAIN:$(MAINDIR)/src/%.cpp=$(MAINDIR)/bin/%)

$(LIB)/obj/%.o : $(LIB)/src/%.cpp
	$(CXX) $(FLAGS) $(INCLUDEPATH) -c $< -o $@

$(MAINDIR)/bin/%: $(MAINDIR)/src/%.cpp $(OBJECTS)
	$(CXX) $(FLAGS) $(INCLUDEPATH) $< -o $@ $(LIBLINK)

all: main

main: $(MAIN) $(INCLUDES) $(SOURCES) $(MKMAIN)

.PHONY:
clean:
	$(RM) $(MKMAIN)

