CC=gcc
CFLAGS1 = -std=c11 -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags glib-2.0` -o tp1
LIBS = `pkg-config --libs libxml-2.0` `pkg-config --libs glib-2.0`
FLEX=flex
FLEXFLAGS1 = -o tp1.c
LIBS = `pkg-config --libs libxml-2.0` `pkg-config --libs glib-2.0`

program:
	$(FLEX) $(FLEXFLAGS1) tp1.l
	$(CC) $(CFLAGS1) tp1.c estruturas.c $(LIBS)

clean:
	rm tp1
	rm tp1.c 
	rm -rf *.dSYM
