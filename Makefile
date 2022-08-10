CC := g++
CCFLAGS ?= -Wall -Werror -std=c++17
DBGFLAGS := -g
CCOBJFLAGS := $(CCFLAGS) -c

BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

TARGET_NAME := a1
TARGET_PATH := $(BIN_PATH)/$(TARGET_NAME)

# list of source files in /src (.cpp files)
SRC := $(foreach file, $(wildcard $(SRC_PATH)/*.cpp), $(notdir $(file)))

# list of obj files based on the above list of source files
# prepended with obj/
# prepend OBJ_PATH and replace ".cpp" with ".o" in all of the items in the SRC variable
OBJ := $(addprefix $(OBJ_PATH)/, $(SRC:.cpp=.o))

default: makedir all

makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

all: $(TARGET_PATH)  # expands to a1

# $(OBJ) expands to all obj files in the /obj directory
# these are named obj/a.o, obj/b.o, etc., and each call the next rule below

# $@ outputs the target (here, $(TARGET_NAME))
# $^ outputs the list of sources (here, $(OBJ))

# this runs after all of the obj files are created below
$(TARGET_PATH): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CCFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) $(CCOBJFLAGS) -o $@ $<

# TODO: CLEAN