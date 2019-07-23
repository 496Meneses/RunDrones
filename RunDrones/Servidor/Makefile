all: servidor.o game.o aleatorio.o
	gcc -o servidor servidor.o game.o aleatorio.o -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -pthread
 
game.o: game.c
	gcc -c -o game.o game.c
aleatorio.o: aleatorio.c
	gcc -c -o aleatorio.o aleatorio.c
servidor.o: servidor.c
	gcc -c -o servidor.o servidor.c

clean:
	rm -rf *.o cliente
	
