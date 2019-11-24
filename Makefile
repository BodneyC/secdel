CC=g++
CFLAGS=-Iinclude -std=c++11
ODIR=obj
OBJ=$(ODIR)/fileinfo.o

$(OBJ): fileinfo.C
	$(CC) -c -o $@ $< $(CFLAGS)

secdel.exe: $(OBJ)
	$(CC) -o $@ $^ secdel.C $(CFLAGS)

.PHONY: clean

clean:
	del $(ODIR)/*.o *~ core 
