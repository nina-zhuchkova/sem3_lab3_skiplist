BINDIR=bin
SRCDIR=src
OBJDIR=obj
INCDIR=inc


CXXFLAGS:=-I .\inc
SRC:=$(wildcard ./$(SRCDIR)/*.cpp)
OBJ:=$(patsubst ./$(SRCDIR)/%.cpp, ./$(OBJDIR)/%.obj, $(SRC)) 

all: $(OBJ)
	g++ $(OBJ) -o ./$(BINDIR)/main

./$(OBJDIR)/%.obj: ./$(SRCDIR)/%.cpp
	g++ -c $^ -o $@ $(CXXFLAGS)

.PHONY: clean
clean:
	del .\$(OBJDIR)\main.obj