CXX = g++
CXXFLAGS = -std=c++11 -g
SFML_PATH = /opt/homebrew/Cellar/sfml/2.6.1
SFML_INCLUDE = $(SFML_PATH)/include
SFML_LIB = $(SFML_PATH)/lib
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
EXEC = checkers

all: $(EXEC)

$(EXEC): main.o
	$(CXX) main.o -o $(EXEC) -L $(SFML_LIB) $(SFML_FLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -I $(SFML_INCLUDE) -c main.cpp

clean:
	rm -f *.o $(EXEC)
