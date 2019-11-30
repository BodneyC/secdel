CC=g++
CFLAGS=-Iinclude -std=c++11
ODIR=obj
OBJ=$(ODIR)/InputFile.o
BIN=bin/secdel.exe

$(BIN): $(OBJ)
	$(CC) -o $@ $^ secdel.C $(CFLAGS)

$(OBJ): InputFile.C
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm $(ODIR)/*.o $(BIN)
