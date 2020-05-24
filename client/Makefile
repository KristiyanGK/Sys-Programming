IDIR=./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./obj
_DEPS=coordinates.h client.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=client.o main.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

startup.o: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o ./*.o