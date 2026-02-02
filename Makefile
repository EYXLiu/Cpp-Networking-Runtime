CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread -I./include
EXEC = nr
SRC = main.cpp ./src/*.cpp

all: $(EXEC)

$(EXEC):
	@$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)

run: $(EXEC)
	@./$(EXEC)

clean:
	@rm -f $(EXEC)