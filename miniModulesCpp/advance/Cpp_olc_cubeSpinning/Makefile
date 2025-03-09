CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lSDL2

SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = cubeV

$(EXEC): $(OBJECTS)
    $(CXX) -o $(EXEC) $(OBJECTS) $(LDFLAGS)

.cpp.o:
    $(CXX) $(CXXFLAGS) -c $<

clean:
    rm -f $(OBJECTS) $(EXEC)
