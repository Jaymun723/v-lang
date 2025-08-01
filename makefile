SRC_DIR := src
BUILD_DIR := build
TARGET_NAME := v-lang
TARGET := $(BUILD_DIR)/$(TARGET_NAME)

# Find all .cpp files recursively in src/
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wuninitialized -pedantic
INCLUDES := -I$(SRC_DIR)

# Create build directories for object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all: $(TARGET)

run: all
	$(BUILD_DIR)/$(TARGET_NAME)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean