.PHONY: all build clean run

# The name of the final executable (must match CMakeLists.txt)
SERVER_EXECUTABLE = server
CLIENT_EXECUTABLE = client

# The build directory
BUILD_DIR = build


build:
	@mkdir -p $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR)

server: build
	@./$(BUILD_DIR)/$(SERVER_EXECUTABLE)

client:
	@./$(BUILD_DIR)/$(CLIENT_EXECUTABLE) $(ARGS)

clean:
	@rm -rf $(BUILD_DIR)
