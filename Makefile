.PHONY: all build clean run

# The name of the final executable (must match CMakeLists.txt)
SERVER_EXECUTABLE = main

# The build directory
BUILD_DIR = build

all: build run

build:
	@mkdir -p $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)

run:
	@./$(BUILD_DIR)/$(SERVER_EXECUTABLE)


clean:
	@rm -rf $(BUILD_DIR)
