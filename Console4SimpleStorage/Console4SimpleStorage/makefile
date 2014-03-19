CC=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
LDFLAGS=
SOURCES=main.cpp BigMoneyAI.cpp CardManager.cpp Game.cpp GameState.cpp Node.cpp NodePool.cpp Player.cpp PlayerState.cpp UCTMonteCarlo.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=uct

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@