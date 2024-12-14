CC = gcc
AS = as
ASFLAGS = -g -march=armv8-a
CFLAGS = -Wall -Wextra -I./inc
LDFLAGS = -lwiringPi -pthread

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

EXEC = f

C_SRCS = $(wildcard $(SRC_DIR)/*.c)
AS_SRCS = $(wildcard $(SRC_DIR)/*.s)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS)) $(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(AS_SRCS))

TARGET = $(BIN_DIR)/$(EXEC)

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)


$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# .PHONY: all clean