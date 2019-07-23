all: main.o  game.o aleatorio.o
	 gcc -o main main.o game.o aleatorio.o -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -pthread
 
game.o: game.c
	gcc -c -o game.o game.c

aleatorio.o: aleatorio.c
	gcc -c -o aleatorio.o aleatorio.c
main.o: main.c
	gcc -c -o main.o main.c

clean:
	rm -rf *.o cliente
	
