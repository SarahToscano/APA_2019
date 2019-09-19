all: apa

apa: main.o vizinhancas.o
	 gcc -o apa main.o vizinhancas.o

main.o: main.o vizinhancas.h
		gcc -c main.c

vizinhancas.o: vizinhancas.c vizinhancas.h
			   gcc -c vizinhancas.c

clean:
	  rm -rf *.o *~ apa