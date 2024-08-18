CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iinclude
LDFLAGS = -std=c++17

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp) main.cpp src/HelperFunctions.cpp
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

EXEC = CommunityCenterManagement

all: $(OBJ_DIR) $(OBJ_DIR)/src $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)/src
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -MMD -o $@ -c $<

$(OBJ_DIR)/src:
	mkdir -p $(OBJ_DIR)/src

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

-include $(DEPS)

clean:
	$(RM) $(EXEC) $(OBJ_DIR)/*

.PHONY: all clean
