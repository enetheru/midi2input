LDFLAGS=-lasound -lX11 -lXtst
CFLAGS= -Wall

mid2key: mid2key.c
	cc $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean: 
	rm -f mid2key
