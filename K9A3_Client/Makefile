CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC_DIR := .
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)
TARGET = main.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@find . -type f \( -name '*.o' -o -name '*.s' -o -name '*.S' -o -name '*.asm' -o -name '*.i' -o -name '*.ii' \) -exec rm -f {} +
	@rm -f $(TARGET)

clean_objs:
	@find . -type f \( -name '*.o' -o -name '*.s' -o -name '*.S' -o -name '*.asm' -o -name '*.i' -o -name '*.ii' \) -exec rm -f {} +

run: $(TARGET)
	$(MAKE) clean_objs
	@echo "Running the program..."
	./$(TARGET)

.PHONY: all clean run
