# -*- MakeFile -*-

COMP=gcc
FLAG=-Wall
EXEC=main

SRC:=src
OBJ:=obj
BIN:=bin
DEP:=.deps

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
DEPS=$(patsubst $(OBJ)/%.o, $(DEP)/%.d, $(OBJS))

-include $(DEPS)
TREE=-MT $@ -MMD -MP -MF $(@:$(OBJ)/%.o=$(DEP)/%.d)

all: $(BIN)/$(EXEC)

build:
	@echo "creating directories..."
	mkdir $(SRC) $(OBJ) $(BIN) $(DEP)
	mv *.c *.h $(SRC)/

$(BIN)/$(EXEC): $(OBJS)
	@echo "compiling $@"
	$(COMP) $(FLAG) $(OBJS) -o $@ -l m

$(OBJ)/%.o: $(SRC)/%.c
	@echo "making object $@"
	$(COMP) $(FLAG) -c $< -o $@ $(TREE)

clean:
	@echo "cleaning up..."
	mv $(SRC)/* $(PWD)/
	rm -rf $(OBJ) $(BIN) $(DEP) $(SRC)
