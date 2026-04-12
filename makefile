CFLAGS = -Wall -std=c11 -I./include -Isrc/internal
CC = gcc
SRC_DIR = src
OBJ_DIR = obj
TESTS_DIR = tests
BIN_DIR = bin
CFLAGS += -O3

SRC_FILES_ALL = $(shell find $(SRC_DIR) -name "*.c")
SRC_FILES = $(filter-out $(SRC_DIR)/main.c, $(SRC_FILES_ALL))
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run-test: $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(TESTS_DIR)/$(T).c -o $(BIN_DIR)/$(T)
	@echo "--- Esecuzione di $(T) ---"
	@time ./$(BIN_DIR)/$(T) $(N)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)