NAME_SERVER := myteams_server
NAME_CLI := myteams_cli
BUILD_DIR := build
CMAKE := cmake
CMAKE_FLAGS := -S . -B $(BUILD_DIR)

.PHONY: all clean fclean re configure

all: configure
	$(CMAKE) --build $(BUILD_DIR)

configure:
	$(CMAKE) $(CMAKE_FLAGS)

clean:
	$(CMAKE) --build $(BUILD_DIR) --target clean 2>/dev/null || true
	rm -rf $(BUILD_DIR)/CMakeFiles $(BUILD_DIR)/cmake_install.cmake $(BUILD_DIR)/Makefile $(BUILD_DIR)/CMakeCache.txt $(BUILD_DIR)/compile_commands.json

fclean: clean
	rm -f $(NAME_SERVER) $(NAME_CLI)

re: fclean all
