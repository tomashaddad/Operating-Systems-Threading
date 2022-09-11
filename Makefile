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

# a callable function that returns cpp files in a directory
list_cpp_in = $(foreach file, $(wildcard $(1)/*.cpp), $(notdir $(file)))
# a callable function that transforms a list of cpp files into a list of obj files
format_obj = $(addprefix $(OBJ_PATH)/$(1)/, $(2:.cpp=.o))

TASK1_CPP = $(call list_cpp_in, $(TASK1_DIR))
TASK2_CPP = $(call list_cpp_in, $(TASK2_DIR))
TASK3_CPP = $(call list_cpp_in, $(TASK3_DIR))
TASK4_CPP = $(call list_cpp_in, $(TASK4_DIR))
COMMON_CPP = $(call list_cpp_in, $(COMMON_DIR))

TASK1_OBJ := $(call format_obj,task1,$(TASK1_CPP))
TASK2_OBJ := $(call format_obj,task2,$(TASK2_CPP))
TASK3_OBJ := $(call format_obj,task3,$(TASK3_CPP))
TASK4_OBJ := $(call format_obj,task4,$(TASK4_CPP))
COMMON_OBJ := $(call format_obj,common,$(COMMON_CPP))

makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATHS) src/task2/sorted src/task3/sorted src/task4/sorted

task1: $(TASK1_TARGET)
task2: $(TASK2_TARGET)
task3: $(TASK3_TARGET)
task4: $(TASK4_TARGET)
# task5: $(TASK5_TARGET)

######### LINKING TASKS

$(TASK1_TARGET): $(COMMON_OBJ) $(TASK1_OBJ)
	$(CC) -o $@ $^ $(CCFLAGS)

# We don't want the main file of task1, so we remove it from the prereq lists

$(TASK2_TARGET): $(COMMON_OBJ) $(filter-out obj/task1/main.o, $(TASK1_OBJ)) $(TASK2_OBJ)
	$(CC) -o $@ $^ $(CCFLAGS)

$(TASK3_TARGET): $(COMMON_OBJ) $(filter-out obj/task1/main.o, $(TASK1_OBJ)) $(TASK3_OBJ)
	$(CC) -pthread -o $@ $^ $(CCFLAGS)

$(TASK4_TARGET): $(COMMON_OBJ) $(filter-out obj/task1/main.o, $(TASK1_OBJ)) $(TASK4_OBJ)
	$(CC) -pthread -o $@ $^ $(CCFLAGS)

######### COMPILING TASKS

$(OBJ_PATH)/task1/%.o: $(TASK1_DIR)/%.cpp
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(OBJ_PATH)/task2/%.o: $(TASK2_DIR)/%.cpp
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(OBJ_PATH)/task3/%.o: $(TASK3_DIR)/%.cpp
	$(CC) -pthread $(CCOBJFLAGS) -o $@ $<

$(OBJ_PATH)/task4/%.o: $(TASK4_DIR)/%.cpp
	$(CC) -pthread $(CCOBJFLAGS) -o $@ $<

$(OBJ_PATH)/common/%.o: $(COMMON_DIR)/%.cpp
	$(CC) $(CCOBJFLAGS) -o $@ $<
