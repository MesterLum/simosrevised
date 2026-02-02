CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main
SRC = src/simos_revised.cpp src/utils.cpp main.cpp

# Test configuration
TEST_DIR = tests
TEST_BUILD_DIR = $(BUILD_DIR)/tests
LIB_SOURCES = src/simos_revised.cpp src/utils.cpp
TEST_UTILS = $(TEST_BUILD_DIR)/test_utils
TEST_SIMOS = $(TEST_BUILD_DIR)/test_simos_revised
TEST_SCENARIOS = $(TEST_BUILD_DIR)/test_scenarios

# Google Test configuration
# Try to detect Homebrew installation (macOS)
GTEST_HOMEBREW_PREFIX := $(shell brew --prefix 2>/dev/null)
ifdef GTEST_HOMEBREW_PREFIX
	GTEST_INCLUDE = -I$(GTEST_HOMEBREW_PREFIX)/include
	GTEST_LIBS = -L$(GTEST_HOMEBREW_PREFIX)/lib -lgtest -lgtest_main -lpthread
else
	# Fallback to standard system paths
	GTEST_INCLUDE = 
	GTEST_LIBS = -lgtest -lgtest_main -lpthread
endif

.PHONY: all run clean test test-utils test-simos test-scenarios test-all build-tests

all: $(BUILD_DIR) $(TARGET)

# Build tests target
build-tests: $(TEST_BUILD_DIR) $(TEST_UTILS) $(TEST_SIMOS) $(TEST_SCENARIOS)

# Test targets
test-utils: $(TEST_UTILS)
	./$(TEST_UTILS)

test-simos: $(TEST_SIMOS)
	./$(TEST_SIMOS)

test-scenarios: $(TEST_SCENARIOS)
	./$(TEST_SCENARIOS)

test-all: test-utils test-simos test-scenarios

test: build-tests test-all

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Build test executables
$(TEST_UTILS): $(TEST_DIR)/test_utils.cpp $(LIB_SOURCES) | $(TEST_BUILD_DIR)
	@echo "Building test_utils..."
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -o $(TEST_UTILS) $(TEST_DIR)/test_utils.cpp $(LIB_SOURCES) $(GTEST_LIBS)

$(TEST_SIMOS): $(TEST_DIR)/test_simos_revised.cpp $(LIB_SOURCES) | $(TEST_BUILD_DIR)
	@echo "Building test_simos_revised..."
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -o $(TEST_SIMOS) $(TEST_DIR)/test_simos_revised.cpp $(LIB_SOURCES) $(GTEST_LIBS)

$(TEST_SCENARIOS): $(TEST_DIR)/test_scenarios.cpp $(LIB_SOURCES) | $(TEST_BUILD_DIR)
	@echo "Building test_scenarios..."
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -o $(TEST_SCENARIOS) $(TEST_DIR)/test_scenarios.cpp $(LIB_SOURCES) $(GTEST_LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

# Help target
help:
	@echo "Simos Revised - Makefile Help"
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build the main application"
	@echo "  make run          - Run the main application"
	@echo "  make build-tests  - Build all unit tests"
	@echo "  make test         - Build and run all tests"
	@echo "  make test-utils   - Run SimosUtils tests"
	@echo "  make test-simos   - Run SimosRevised tests"
	@echo "  make test-scenarios - Run 10 scenario tests"
	@echo "  make test-all     - Run all tests"
	@echo "  make clean        - Remove build directory"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Note: Tests require Google Test library"
	@echo "      On macOS: brew install googletest"
	@echo "      On Ubuntu/Debian: sudo apt-get install libgtest-dev"
