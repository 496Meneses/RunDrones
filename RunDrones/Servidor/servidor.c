#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>

#include <arpa/inet.h>

#include <raylib.h>

#include "game.h"


//metodos
void * handle_connection(int c);

int main(int argc, char*argv[]) {
    int s;
    int c;
    struct sockaddr_in addr;
    int valopc;
    socklen_t len;
    if ( (s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
}

valopc=1;
len = sizeof(valopc);
setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void*)&valopc,len);
memset(&addr, 0, sizeof(struct sockaddr_in));
addr.sin_family = AF_INET;
addr.sin_port = htons(5555);//TODO argumento del main! /////////////////////////////
addr.sin_addr.s_addr = INADDR_ANY;
if (bind (s, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) != 0) {
perror("bind");
exit(EXIT_FAILURE);
}

if (listen(s, 10) != 0) {
    perror("bind");
    exit(EXIT_FAILURE);
}
pthread_t hilo;
//Recibir la conexion del cliente
c = accept(s, NULL, 0);
//Atender la conexion
   /*pthread_create(&hilo, NULL, handle_connection, (void *)&c);
   pthread_join(hilo,NULL);*/
   handle_connection(c);
close(s);
exit(EXIT_SUCCESS);
}
     


    void * handle_connection (int  jugador)
    {
        char buf[512];
		strcpy(buf, "Hello!");
        write(jugador, buf, 512);
            Game game;
	//Initialize game
	Initialize(&game);
	
	// Detect window close button or quit key
	while (!WindowShouldClose())    
	{ 
		//Update
        		
		ProcessEvents(&game,jugador);

		// Draw
		DrawGame(&game);		
	}
	
	//Finalize game
	Finalize(&game);
	
	CloseWindow();// Close window and OpenGL context
	
    }