# Max DeLiso <me@maxdeliso.name>
# BST implementation: makefile for nix systems

.PHONY: clean

RM=rm
CC=clang
CFLAGS=-Wall

OUT=bst_tree_test
MODULES=bst_tree bst_tree_test

OBJ=$(addsuffix .o, $(MODULES))

$(OUT): $(OBJ)
	$(CC) -g $(OBJ) -o $(OUT)

.c.o:
	$(CC) $(CFLAGS) -c -g $<

clean:
	$(RM) -f $(OBJ) $(OUT)
