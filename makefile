SRC_DIR := src
BUILD_DIR := build
TEST_DIR := test
TARGET_NAME := v-lang
TARGET_SRC := $(SRC_DIR)/$(TARGET_NAME).c
TARGET := $(BUILD_DIR)/$(TARGET_NAME) 
TARGET_OBJ := $(BUILD_DIR)/$(TARGET_NAME).o

# Compiler and flags
CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wuninitialized -pedantic -fsanitize=address -fno-omit-frame-pointer
INCLUDES := -I$(SRC_DIR)

# Source files (exclude v-lang.c for tests)
SRCS := $(filter-out $(TARGET_SRC), $(shell find $(SRC_DIR) -name '*.c'))
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))


# Default target: build the app
all: $(TARGET)

# Build main executable
$(TARGET): $(OBJS) $(TARGET_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# Build main file separately
$(TARGET_OBJ): $(TARGET_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create build directories for object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run the app
run: all
	$(TARGET)

test-%: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	@if [ ! -f $(TEST_DIR)/test_$*.c ]; then \
		echo "Error: $(TEST_DIR)/test_$*.c does not exist!"; \
		exit 1; \
	fi
	@echo "Building test for $*..."
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(TEST_DIR)/test_$*.c -o $(BUILD_DIR)/test_$*
	@echo "Running test_$*..."
	$(BUILD_DIR)/test_$*

clean:
	rm -rf $(BUILD_DIR)

gen: test-generator
	hd test.wasm > test.hd.txt
	$(HOME)/code/wabt/bin/wasm2wat test.wasm -o test.wat

.PHONY: all run clean