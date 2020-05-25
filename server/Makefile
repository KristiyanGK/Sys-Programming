IDIR=./include
CC=gcc
CFLAGS=-I$(IDIR) -lpthread

ODIR=./obj
_DEPS=coordinates.h server.h queue.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o server.o queue.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

startup.o: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o ./*.o