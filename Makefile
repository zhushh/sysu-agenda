DIR_SRC=./src
DIR_OBJ=./obj
DIR_HEADER=./headers
DIR_BIN=./bin

SRC=$(wildcard $(DIR_SRC)/*.cpp)
OBJ=$(patsubst %.cpp,$(DIR_OBJ)/%.o,$(notdir $(SRC)))

TARGET1=server
TARGET2=client
BIN_TARGET1=$(DIR_BIN)/$(TARGET1)
BIN_TARGET2=$(DIR_BIN)/$(TARGET2)
CC=g++
CCFLAGS= -Wall -std=c++0x

$(TARGET1): $(OBJ)
	rm $(DIR_OBJ)/client.o
	$(CC) $(CCFLAGS) $(DIR_OBJ)/*.o -o $(BIN_TARGET1)

$(TARGET2): $(OBJ)
	rm $(DIR_OBJ)/server.o
	$(CC) $(CCFLAGS) $(DIR_OBJ)/*.o -o $(BIN_TARGET2)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp $(DIR_HEADER)/*.h
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -rf $(DIR_OBJ)/*.o $(BIN_TARGET1) $(BIN_TARGET2) $(DIR_BIN)/*.csv
