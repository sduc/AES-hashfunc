CC=gcc
PROF=-fno-pie -g -pg
CFLAGS=-I$(LDIR) -std=c99 -O2
LDIR=aes
LIBS=-maes -msse4 -mpclmul
_OBJ=aes_encryption.o aes_key_expansion.o
OBJ=$(patsubst %, $(LDIR)/%, $(_OBJ))
SPEFLAG=-std=c99 -I. -O2


all: cpu_support $(OBJ) hashit perf test/test_field_op prof

prof: prof.c hash.o $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(SPEFLAG) 

test/test_field_op:test/test_field_op.c field_op.h
	$(CC) -o $@ $^ $(LIBS) $(SPEFLAG)

perf: perf.c hash.o $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

hashit: hashit.c hash.o $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

cpu_support: cpu_support.c
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

hash.o: hash.c hash.h $(LDIR)/aes.h
	$(CC) -c -o $@ $< $(LIBS) $(CFLAGS)

$(LDIR)/%.o: $(LDIR)/%.c  $(LDIR)/aes.h 
	$(CC) -c -o $@ $< $(LIBS) $(CFLAGS)

.PHONY: clean
	
clean:
	rm -f *.o *~ core $(LDIR)/*~ $(LDIR)/*.o

