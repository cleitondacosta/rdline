# Very complex makefile
CC=gcc
SRC=src/main.c
BIN=bin/rdline
FLAGS=
STD=

all:
	$(CC) -Wall $(SRC) $(FLAGS) -o $(BIN) $(STD)
