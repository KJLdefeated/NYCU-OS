# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

# Libraries
LIBS = -lssl -lcrypto -lstdc++fs

# Target executable
TARGET = 110652019_hw6

# Source files
SRC = deduplicate.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

test: $(TARGET)
	./demo.sh $(TARGET)

clean:
	rm -f $(TARGET)