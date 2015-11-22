LDFLAGS=-lX11 -lXtst -ljack
CFLAGS=-Wall

midi2input: midi2input.c
	g++ $(CFLAGS) $^ -o $@ $(LDFLAGS)
clean: 
	rm -f midi2input
