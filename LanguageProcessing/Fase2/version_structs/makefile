CC=gcc
CFLAGS1 = -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags glib-2.0` 
LIBS = `pkg-config --libs libxml-2.0` `pkg-config --libs glib-2.0`

GIC.exe : y.tab.o lex.yy.o
			gcc -o GIC.exe $(CFLAGS1) y.tab.o lex.yy.o $(LIBS) -ll 

y.tab.o : y.tab.c
		gcc -c $(CFLAGS1) y.tab.c $(LIBS) 

y.tab.c y.tab.h: GIC.y
	yacc -d GIC.y
	
lex.yy.o : lex.yy.c
	gcc -c lex.yy.c


lex.yy.c : PLFlex.l y.tab.h
	flex PLFlex.l


program: 
		$(CC) $(CFLAGS1) estruturas.c $(LIBS)

clean :
			rm -f GIC.exe
			rm lex.yy.*
			rm y.tab.*
			rm log.json
