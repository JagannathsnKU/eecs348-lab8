CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = calculator
OBJS = main.o Calculator.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp Calculator.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
