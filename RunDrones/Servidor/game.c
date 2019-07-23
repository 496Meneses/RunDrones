#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#include "game.h"
#include "aleatorio.h"
void Initialize(Game * game) {
	// Inicializacion
	game->screen.width = 800;
	game->screen.height = 450;
	
	// Carga las imagenes en RAM	
	Image mapImage = LoadImage("resources/map.jpg"); 	
	Image siteImage = LoadImage("resources/site.png");	
	Image droneImage = LoadImage("resources/drone.png");
	
	/* Obtener el tamano del mapa a partir de la imagen cargada */
	game->map.width = mapImage.width;
	game->map.height = mapImage.height;
	
	//Raylib: inicializar la ventana del juego
	InitWindow(game->screen.width, game->screen.height, "Drones");
	
	//Calcular los niveles de zoom en X y Y de acuerdo con el tama�o del mapa y de la pantalla
	double xZoom = (double)game->screen.width / game->map.width;
	double yZoom = (double)game->screen.height / game->map.height;
	
	//Calcular el zoom m�nimo para la c�mara
	game->minZoom = xZoom;	
	if (yZoom > game->minZoom) {
		game->minZoom = yZoom;
	}
	
	//M�ximo nivel de zoom
	game->maxZoom = 1.5f;
	
	//Raylib: Cambiar la tecla de salida de ESC a Q
	SetExitKey(KEY_Q);
	
	//Habilitar gestos de entrada (mouse, touch)
	SetGesturesEnabled(GESTURE_DRAG);
	
	// Crear las texturas y transferirlas en la memoria de video
	game->map.texture = LoadTextureFromImage(mapImage);  
	game->siteTexture = LoadTextureFromImage(siteImage);
	
	
	
	//Cargar la imagen para el dron
	game->droneTexture = LoadTextureFromImage(droneImage);
	
	//printf("Dimensiones de la imagen: %d %d\n", mapImage.width, mapImage.height);
	game->map.base = (Rectangle){0, 0,  mapImage.width, mapImage.height};
	
	
	// Las imagenes ya se han cargado a la memoria de video, se pueden quitar de la RAM
	UnloadImage(mapImage);   	
	UnloadImage(siteImage);
	UnloadImage(droneImage);
	
	//Cantidad maxima de FPS (Frames Per Second) del juego
	SetTargetFPS(60);
	
	//Crear la c�mara
	
	game->camera = (Camera2D){ 0 };
	game->camera.target = (Vector2){ game->map.base.x, game->map.base.y};
	game->camera.offset = (Vector2){ -1 * game->map.base.x, -1* game->map.base.y };
	game->camera.rotation = 0.0f;
	
	//El zoom inicial es el zoom definido para el juego
	game->camera.zoom = game->minZoom;
	//--------------------------------------------------------------------------------------
	
	//Estado inicial del juego
	game->state = EXPLORING;
	
	//Cargar la fuente
	game->font = LoadFont("resources/Aller_Rg.ttf");
	
	//Inicializar el arreglo din�mico de drones. Inicialmente vac�o.
	game->drones = NULL;
    game->numDrones = 0;
    game->numSites=0;

	//Establecer el filtro para el trazado de la fuente
	SetTextureFilter(game->font.texture, FILTER_BILINEAR);
}
void generarCoordenadas(Game * game, int id)
{
            Drone * aux = malloc(2 * sizeof(Drone));
            if (game->drones != NULL) {
					memcpy(aux, game->drones, game->numDrones * sizeof(Drone));
					//Liberar la memoria del arreglo original
					free(game->drones);
					
			}
            game->drones = aux;	
			game->drones[0].position.x = (float)aleatorio(); //YO
			game->drones[0].position.y = (float)aleatorio();

            game->drones[1].position.x = (float)aleatorio(); //OPONENTE
			game->drones[1].position.y = (float)aleatorio();
			
			game->drones[0].state = DRONE_STATIONARY; // servidor
			game->drones[0].width = game->droneTexture.width;
			game->drones[0].height = game->droneTexture.height;
			game->drones[0].rotation = 0.0f;

		    game->drones[1].state = DRONE_STATIONARY; // cliente
			game->drones[1].width = game->droneTexture.width;
			game->drones[1].height = game->droneTexture.height;
			game->drones[1].rotation = 0.0f;	

            game->numDrones  = 2;

			game->site.x= (float)aleatorio() ; 
    	    game->site.y = (float)aleatorio() ;
        	game->numSites = 1;

			printf("corenada dron 0 posicion x %f\n",game->drones[0].position.x);
			printf("corenada dron 0 posicion y %f\n",game->drones[0].position.y);
										printf("corenada dron 1 posicion x %f\n",game->drones[1].position.x);
										printf("corenada dron 1 posicion y %f\n",game->drones[1].position.y);
										printf("corenada sitio 0 posicion x %f\n",game->site.x);
										printf("corenada sitio 0 posicion y %f\n",game->site.y);

			char buf[512];
			sprintf(buf,"%f",game->drones[0].position.x);
			write(id, buf, 512); 

			sprintf(buf,"%f",game->drones[0].position.y);
			write(id, buf, 512); 

			sprintf(buf,"%f",game->drones[1].position.x);
			write(id, buf, 512); 

			sprintf(buf,"%f",game->drones[1].position.y);
			write(id, buf, 512); 

			sprintf(buf,"%f",game->site.x);
			write(id, buf, 512); 

			sprintf(buf,"%f",game->site.y);
			write(id, buf, 512); 


}
void ProcessEvents(Game * game, int id) {
	
	//La tecla "R" reinicia el juego
	if(IsKeyPressed(KEY_R)) {
    
		ResetGame(game);

	}	
	//SI PRECIONA G INICIA JUEGO
	if (IsKeyPressed(KEY_G) && game->state != GAMING) {
		
		char buf[512];
		strcpy(buf,"jugar");
		write(id, buf, 512);

		generarCoordenadas(game,id);

		game->state = GAMING;
		game->camera.offset.x = 0.0f;
		game->camera.offset.y = 0.0f;
		game->camera.zoom = game->minZoom;
		game->map.base.x = 0.0f;
		game->map.base.y = 0.0f;
		
		// El target de la camara sigue al mapa
		game->camera.target = (Vector2){ game->map.base.x, game->map.base.y};
		game->camera.offset = (Vector2){ -1.0f * game->map.base.x, -1.0f * game->map.base.y };
    }
	if (game->state ==GAMING) {		
		fd_set read_fds;
		struct timeval t; 
		t.tv_sec = 0;
		t.tv_usec = 10;
		FD_ZERO(&read_fds);
		FD_SET(id,&read_fds);
		char buf[512];
		if(select ((id + 1),&read_fds,0 ,0,&t)>=0){	 // HAY DATOS ??
			if (FD_ISSET(id,&read_fds))
			{
					       //Recibir datos
    						memset(buf, 0, 512);
    						read(id, buf, 512);
							if (strstr(buf,"llegue"))
							{
								game->state = FINISHED;
							}
							if(strstr(buf,"moviendomederecha"))
							{
								memset(buf, 0, 512);
    							read(id, buf, 512);
								game->drones[1].position.x = atoi(buf);
							}
							if(strstr(buf,"moviendomeizquierda")){
								memset(buf, 0, 512);
    							read(id, buf, 512);
								game->drones[1].position.x = atoi(buf);
							}
							if(strstr(buf,"moviendomearriba")){
								memset(buf, 0, 512);
    							read(id, buf, 512);
								game->drones[1].position.y = atoi(buf);
							}
							if(strstr(buf,"moviendomeabajo")){
								memset(buf, 0, 512);
    							read(id, buf, 512);
								game->drones[1].position.y = atoi(buf);
							}
											
			}
		}

		//MOVIMIENTO DEL DRON
        if (IsKeyDown(KEY_RIGHT)){
			//actualizar mi estado notificar estado
			game->drones[0].position.x += 15.0f;
			strcpy (buf,"moviendomederecha");
			write(id, buf, 512);
			sprintf(buf,"%f",game->drones[0].position.x);
			write(id, buf, 512);
		}
        if (IsKeyDown(KEY_LEFT)){ 
			//actualizar mi estado notificar estado
			game->drones[0].position.x  -= 15.0f;
			strcpy (buf,"moviendomeizquierda");
			write(id, buf, 512);
			sprintf(buf,"%f",game->drones[0].position.x);
			write(id, buf, 512); 
		}
        if (IsKeyDown(KEY_UP)){ 
			//actualizar mi estado notificar estado
			game->drones[0].position.y -= 15.0f;
			strcpy (buf,"moviendomearriba");
			write(id, buf, 512);
			sprintf(buf,"%f",game->drones[0].position.y);
			write(id, buf, 512); 
			}
        if (IsKeyDown(KEY_DOWN)){ 
			//actualizar mi estado notificar estado
			game->drones[0].position.y += 15.0f;
			strcpy (buf,"moviendomeabajo");
			write(id, buf, 512);
			sprintf(buf,"%f",game->drones[0].position.y);
			write(id, buf, 512);  
		}

			// LLEGADA
                if ((game->drones[0].position.x >= (game->site.x - 60.0f))
				&&  (game->drones[0].position.x <=game->site.x + 60.0f)
				&& (game->drones[0].position.y >= (game->site.y - 60.0f))
				&&  (game->drones[0].position.y <=game->site.y+ 60.0f))
				{
            		game->site.state = SITE_VISITED;
            		game->state = FINISHED;
					strcpy(buf,"llegue");
					write(id, buf, 512);  
        		}
         
	}
}
void Finalize(Game * game) {
	//Liberar las texturas de la memoria de video
	UnloadTexture(game->map.texture);
	UnloadTexture(game->siteTexture);
	UnloadTexture(game->droneTexture);
	UnloadFont(game->font);
	//Liberar el arreglo de drones
	free(game->drones);
}

void DrawGame(Game * game) {
	
	//Raylib: iniciar el modo de dibujo
	BeginDrawing();
	
	//Limpiar la ventana
	ClearBackground(RAYWHITE);
	
	//Comenzar el modo de camara, en el cual se maneja el acercamiento y desplazamiento del mapa
	BeginMode2D(game->camera);
	
	//Dibujar el mapa dentro del modo de camara
	DrawTexture(game->map.texture, 0, 0, WHITE);
	
	//Salir del modo camara para pintar los demas elementos
	EndMode2D();
	
	

	if (game->numSites > 0){
        Color siteColor = RED;
	    if (game->site.state == SITE_VISITING) {
				siteColor = YELLOW;
	    }else if (game->site.state == SITE_VISITED) {
				siteColor = GREEN;
	    }   
	//Dibujar la textura del sitio en su posici�n, teniendo en cuenta el zoom de la camara y la posicion del mapa
	    DrawTexture(game->siteTexture, 
        (int)((game->site.x - game->map.base.x) * game->camera.zoom) - game->siteTexture.width / 2,  
        (int)((game->site.y - game->map.base.y) * game->camera.zoom) - 32, siteColor);
    }
	
	
	// Mostrar el texto de ayuda
	/* if (game->state == GAMING) {
		DrawStatusText(game, "Con las flechas mover el dron,R para reiniciar,Q para salir", 16, YELLOW);
	}else if (game->state == EXPLORING) {		
		DrawStatusText(game, "R Para iniciar juego", 16, YELLOW);
	}else if (game->state == FINISHED) {
		DrawStatusText(game, "Juego finalizado. Presione R para reiniciar, Q para salir.", 16, YELLOW);
	}*/
	
	//Dibujar los drones
    if (game->numDrones > 0){
	      for (int i = 0; i < 2; i++){ 				
		    	DrawDrone(game, &game->drones[i], 0.15f);
		    }
	    }
    
	
	//Raylib: finalizar el modo de dibujo
	EndDrawing();
}
void DrawDrone(Game * game, Drone *drone, float scale) {
	
	//Solo se dibujan los drones usados!!
	if (drone->state == DRONE_UNUSED) {
		return;
	}

	//Rectangulo base (parte de la textura que se usar� para dibujar el dron = toda la imagen)
	Rectangle sourceRec = { 0.0f, 0.0f, drone->width, drone->height};
	
	//Rect�ngulo destino: Ubicaci�n del dron en la ventana, con el zoom adecuado
	Rectangle destRec = { 
		(int)((drone->position.x - game->map.base.x) * game->camera.zoom),
		 (int)((drone->position.y - game->map.base.y) * game->camera.zoom), 
			drone->width * scale, 
			drone->height * scale
	};
	
	//Origen de rotaci�n
	Vector2 origin = {
		(drone->width * scale) / 2,
		(drone->height * scale) / 2
	};
	
	//Dibujar la textura del dron con los datos de ubicacion, zoom y rotacion calculados.
	DrawTexturePro(game->droneTexture, sourceRec, destRec, origin, drone->rotation, WHITE);
}
void ResetGame(Game * game) {
	if (game->numSites!= 0) {
		game->numSites = 0;
        game->site.state =SITE_UNVISITED;

	}
	
	if (game->numDrones != 0) {
		free(game->drones);
		game->drones = NULL;
		game->numDrones = 0;
	}
	game->map.base.x = 0;
	game->map.base.y = 0;
	game->camera.zoom = game->minZoom;
	game->state = EXPLORING;
}

