CXX=g++
CXXFLAGS= -std=c++0x -Wall -pedantic
LDFLAGS=
SOURCES=main.cpp BigMoneyAI.cpp CardManager.cpp Game.cpp GameState.cpp HumanInputAI.cpp Move.cpp Node.cpp Player.cpp PlayerState.cpp UCTMonteCarlo.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=dominionuct

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@