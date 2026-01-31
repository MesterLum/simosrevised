CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main
SRC = src/simos_revised.cpp src/utils.cpp main.cpp

.PHONY: all run clean

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
