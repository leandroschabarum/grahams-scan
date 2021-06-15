# -*- MakeFile -*-

COMP=gcc
FLAG=-Wall
EXEC=run

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS)
	@echo "compiling $@"
	$(COMP) $(FLAG) $(OBJS) -o $@ -l m

%.o: %.c
	@echo "making object $@"
	$(COMP) $(FLAG) -c $< -o $@

clean:
	clear
	@echo "cleaning up..."
	rm *.o $(EXEC)
