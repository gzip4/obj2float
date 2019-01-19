
all: obj2float

clean:
	rm -f *.[oasid]

obj2float: obj2float.o
	$(CC) -o $@ obj2float.o

.c.o:
	$(CC) -c -o $@ $(CFLAGS) $<
