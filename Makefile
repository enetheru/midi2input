LDFLAGS=-lX11 -lXtst -ljack
CFLAGS=-Wall

mid2key: mid2key.c
	cc $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean: 
	rm -f mid2key
