# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g -O0 -Iinclude -Ilib

# Source files
SRCS = src/main.cpp \
       src/read_data.cpp \
       src/statistics.cpp \
       src/optimizer.cpp \
       src/backtester.cpp \
       lib/csv.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = portfolioSolver

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Rule to clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)
