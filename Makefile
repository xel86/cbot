CC := gcc
CFLAGS := -fPIC -Wall -O3 -Iinclude
LDFLAGS := -Lbin/obj
LDLIBS := -lm -lpthread

SRC_DIR := src
OBJ_DIR := bin/obj
BIN_DIR := bin
COMMANDS_DIR := commands

# Find all .c files in src/ directory
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding object file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Find all .c files in commands/ directory
COMMANDS_SRC := $(wildcard $(COMMANDS_DIR)/*.c)

# Generate corresponding shared library names
COMMANDS_SO := $(patsubst $(COMMANDS_DIR)/%.c,$(COMMANDS_DIR)/%.so,$(COMMANDS_SRC))

.PHONY: all clean commands

all: $(BIN_DIR)/cbot commands

$(BIN_DIR)/cbot: $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

commands: $(COMMANDS_SO)

$(COMMANDS_DIR)/%.so: $(COMMANDS_DIR)/%.c
	@mkdir -p $(COMMANDS_DIR)
	$(CC) $(CFLAGS) -shared -fPIC $< $(filter-out $(OBJ_DIR)/main.o,$(OBJ_FILES)) -o $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR) $(COMMANDS_SO)