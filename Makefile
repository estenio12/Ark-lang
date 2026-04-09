# Variáveis para facilitar a manutenção
CXX = g++
CXXFLAGS = -std=c++23 -I ./src
SRC = $(wildcard ./src/*.cpp)
TARGET = ./test/arkc

# Regra principal
all:
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET)

# Regra para limpar o executável
clean:
	rm -f $(TARGET)

run-test: all
	$(TARGET) abc123
