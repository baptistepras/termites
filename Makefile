tests: tests.o monde.o coord.o grille.o termite.o
	g++ -o tests tests.o monde.o coord.o grille.o termite.o
    
projet: projet.o monde.o coord.o grille.o termite.o
	g++ -o projet projet.o monde.o coord.o grille.o termite.o
    
all: all
	./tests
	./projet
    
main: projet
	./projet

check: tests
	./tests

clean:
	rm -f *.o projet tests
	rm -f core*