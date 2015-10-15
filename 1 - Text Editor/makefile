# This make file was adopted to run in a MinGW environment.  Assumes your current working
# directory is the root of your project.

CXX       = g++
CXXFLAGS  = -g3 -O0 -ansi -std=c++14 -pedantic -Wall -Wold-style-cast -Woverloaded-virtual -Wextra -I. -DUSING_TOMS_SUGGESTIONS
SOURCES   = $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp) $(wildcard */*/*/*/*.cpp)
args      =

.PHONY: project_$(CXX).exe clean
project_$(CXX).exe: $(SOURCES)
	@echo $(SOURCES)
	@$(CXX) --version
	@$(CXX) $(CXXFLAGS) $(args) $(SOURCES) -o $@

clean:
	rm -rf project_$(CXX).exe *.o *.obj
# options to consider:
#       -Weffc++