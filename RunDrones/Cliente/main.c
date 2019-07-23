#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include <raylib.h>

#include "game.h"


void * handle_connection(int s);

int main(int argc, char*argv[]) {
int s;
struct sockaddr_in addr;
if ( (s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
perror("socket");
exit(EXIT_FAILURE);
}
memset(&addr, 0, sizeof(struct sockaddr_in));
addr.sin_family = AF_INET;
addr.sin_port = htons(5555);
//TODO argumento del main!
//TODO tomar la IP de los argumentos del main!!
if (inet_aton("127.0.0.1",&addr.sin_addr) == 0) {
fprintf(stderr, "Invalid address!\n");
exit(EXIT_FAILURE);
}
if (connect (s, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) != 0) {
perror("connect");
exit(EXIT_FAILURE);
}
pthread_t hilo;
        /* pthread_create(&hilo, NULL, handle_connection, (void *)&s);
        pthread_join(hilo, NULL);*/
        handle_connection(s);
close(s);
exit(EXIT_SUCCESS);
}
void * handle_connection(int s){

    char buf[512];
    int finished;
    //Recibir mensaje de bienvenida del servidor
    memset(buf, 0, 512);
    read(s, buf, 512);
    printf("Server sent: %s\n", buf);

    Game game;
	
	//Initialize game
	Initialize(&game);
	
	// Detect window close button or quit key
	while (!WindowShouldClose())    
	{ 

		//Update		
		ProcessEvents(&game,s);

		// Draw
		DrawGame(&game);		
	}
	
	//Finalize game
	Finalize(&game);
	
	CloseWindow();// Close window and OpenGL context
	

}