CC := g++
CCFLAGS ?= -Wall -Werror -std=c++17
DBGFLAGS := -g
CCOBJFLAGS := $(CCFLAGS) -c

BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

OBJ_PATHS := \
	obj/common \
	obj/task1 \
	obj/task2 \
	obj/task3 \
	obj/task4 \
	obj/task5

TARGET_NAME := a1
TARGET_PATH := $(BIN_PATH)/$(TARGET_NAME)

COMMON_DIR := $(SRC_PATH)/common
TASK1_DIR := $(SRC_PATH)/task1
TASK2_DIR := $(SRC_PATH)/task2
TASK3_DIR := $(SRC_PATH)/task3
TASK4_DIR := $(SRC_PATH)/task4
TASK5_DIR := $(SRC_PATH)/task5

TASK1_TARGET := $(BIN_PATH)/task1
TASK2_TARGET := $(BIN_PATH)/task2
TASK3_TARGET := $(BIN_PATH)/task3
TASK4_TARGET := $(BIN_PATH)/task4
TASK5_TARGET := $(BIN_PATH)/task5

SRC := $(foreach file, $(wildcard $(SRC_PATH)/*.cpp), $(notdir $(file)))
OBJ := $(addprefix $(OBJ_PATH)/, $(SRC:.cpp=.o))

list_cpp_in = $(foreach file, $(wildcard $(1)/*.cpp), $(notdir $(file)))
format_obj = $(addprefix $(OBJ_PATH)/$(1)/, $(2:.cpp=.o))

TASK1_CPP = $(call list_cpp_in, $(TASK1_DIR))
TASK2_CPP = $(call list_cpp_in, $(TASK2_DIR))
COMMON_CPP = $(call list_cpp_in, $(COMMON_DIR))

TASK1_OBJ := $(call format_obj,task1,$(TASK1_CPP))
TASK2_OBJ := $(call format_obj,task2,$(TASK2_CPP))
COMMON_OBJ := $(call format_obj,common,$(COMMON_CPP))

makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATHS) $(DBG_PATH)

task1: $(TASK1_TARGET)
task2: $(TASK2_TARGET)
task3: $(TASK3_TARGET)
task4: $(TASK4_TARGET)
task5: $(TASK5_TARGET)

$(TASK1_TARGET): $(COMMON_OBJ) $(TASK1_OBJ)
	$(CC) -o $@ $^ $(CCFLAGS)

$(TASK2_TARGET): $(COMMON_OBJ) $(filter-out obj/task1/main.o, $(TASK1_OBJ)) $(TASK2_OBJ)
	$(CC) -o $@ $^ $(CCFLAGS)

$(OBJ_PATH)/task1/%.o: $(TASK1_DIR)/%.cpp
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(OBJ_PATH)/task2/%.o: $(TASK2_DIR)/%.cpp
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(OBJ_PATH)/common/%.o: $(COMMON_DIR)/%.cpp
	$(CC) $(CCOBJFLAGS) -o $@ $<
