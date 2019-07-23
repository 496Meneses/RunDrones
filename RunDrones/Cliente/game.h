#ifndef GAME_H
#define GAME_H

/* Biblioteca para crear videojuegos. */
#include <raylib.h>
/** 
* @brief Driecci�n en la cual se balancea el dron en posici�n estacionaria.
*/
typedef enum {
	ROTATION_CLOCKWISE, /**< En la direccion de las manecillas del reloj */
	ROTATION_COUNTERCLOCKWISE /**< En direccion contraria a las manecillas del reloj */
}RotationDirection;

/**
* @brief Estado del juego.
*/
typedef enum {	
    LOCATING_SITES,
    LOCATING_DRONE,
	EXPLORING, /**< El usuario se encuentra navegando por el mapa. */
	GAMING, /**< El usuario se encuentra en carrera */
	FINISHED	/** < Juego finalizado. */
}GameState;

/** 
* @brief Estado del dron.
*/
typedef enum {
	DRONE_STATIONARY, /**< El dron se encuentra estacionario, y se balancea en el aire. */
	DRONE_MOVING, /**< El dron se encuentra entregando un paquete. */
	DRONE_UNUSED /**< El dron no se ha usado*/
}DroneState;

/** 
* @brief Estado de un sitio.
*/
typedef enum {
	SITE_UNVISITED, /**< El sitio no ha sido visitado. */
	SITE_VISITING, /**< El sitio se est� visitando. */
	SITE_VISITED /**< El sitio ha sido visitado. */
}SiteState;

/** 
* @brief Informaci�n para un sitio.
*/
typedef struct {
	float x; /**< Coordenada X del sitio. */
	float y; /**< Coordenada Y del sitio. */
	SiteState state; /**< Estado del sitio. */
}Site;

/** 
* @brief Informaci�n para un dron.
*/
typedef struct {
	DroneState state;  /**< Estado del dron. */
	Texture2D texture; /**< Imagen del dron. */
	Vector2 position; /**< Posici�n X Y del dron. */
	int width; /**< Ancho (en pixels) para dibujar. */
	int height; /**< Alto (en pixels) para dibujar. */
	float rotation; /**< Rotaci�n (0.0 - 1.0). */
	RotationDirection rotationDirection; /**< Direcci�n de la rotaci�n. */
	float rotationLimit; /**< L�mite de la rotaci�n. */
	Site * destination; /**< Referencia al sitio de destino asignado. */
}Drone;

/** 
* @brief Pantalla del juego.
*/
typedef struct {
	int width; /**< Ancho de la pantalla en pixels. */
	int height; /**< Alto de la pantalla en pixels. */
}GameScreen;

/** 
* @brief Mapa del juego
*/
typedef struct {
	Texture2D texture;	/**< Imagen de fondo. */
	int width; /**< Ancho del mapa. */
	int height; /**< Alto del mapa. */
	Rectangle base;	/**< Rect�ngulo base del mapa. */
}GameMap;


/** 
* @brief Estructura de informaci�n para el juego.
*/
typedef struct {
	
	GameState state; /**< Estado del juego. */
	GameMap map; /**< Mapa del juego. */
	
	Texture2D siteTexture; /**< Imagen para los sitios. */
	
	Texture2D droneTexture; /**< Imagen para el dron. */

	Camera2D camera; /**< C�mara para permitir acercar / alejar (zoom). */
	
	Drone * drones; /**< Referencia al arreglo din�mico de drones. */
	
	int droneLocated; /**< Bandera que se activa cuando se posiciona un dron. */
	
	GameScreen screen; /**< Pantalla del juego. */
	
	double minZoom; /**< Nivel minimo de acercamiento. */
	double maxZoom; /**< Nivel maximo de acercamiento. */
	int numDrones;
    int numSites;
	Font font; /**< Fuente para dibujar los textos. */
	
	Site site; /**< Referencia al sitio */
	
}Game;
/**
* @brief Inicializaci�n del juego
* 
* En esta subrutina se inicializa la estructura del juego: se cargan los recursos
* y se configuran el mapa, la pantalla, el zoom y dem�s par�metros que se usan
* en el juego.
* @param game Referencia a la estructura con la informaci�n del juego.
*/
void Initialize(Game * game);

/**
* @brief Proceso de los eventos.
* 
* En esta subrutina se procesan los diferentes eventos del juego: pulsaci�n de
* teclas, movimientos y clicks del mouse, etc.
* Cada evento se procesa de acuerdo con el estado en el cual se encuentra el 
* juego.
* @param game Referencia a la estructura con la informaci�n del juego.
*/
void ProcessEvents(Game * game,int id);

/**
* @brief Finaliza el juego.
*
* Se invoca al finalizar el juego, libera los recursos asociados.
* @param game Referecia a la estructura con la informaci�n del juego.
*/
void Finalize(Game * game);

/**
* @brief Ciclo principal de dibujo del juego.
* Actualiza lo que se muestra al usuario, de acuerdo con el estado del juego, los sitios y los drones
* @param game Referecia a la estructura con la informaci�n del juego.
*/
void DrawGame(Game * game);

/**
* @brief Dibuja un dron en un escenario de juego, con una escala
* @param game Referecia a la estructura con la informaci�n del juego.
* @param drone Dron a dibujar
* @param scale Escala de la imagen (0.0-1.0)
*/
void DrawDrone(Game * game, Drone * drone, float scale);

/**
* @brief Dibuja un exto en la esquina superior de la pantalla
* @param game Referencia a la estructura de informaci�n del juego.
* @param text Texto a dibujar
* @param size Tama�o de la fuente
* @param color Color de la fuente
*/
void DrawStatusText(Game * game, const char * text, int size, Color color);

/**
* @brief Mueve un dron dentro del juego
* @param game Referencia a la estructura de informaci�n del juego.
* @param drone Dron a mover.
*/
void MoveDrone(Game * game, Drone * drone);

/**
* @brief Reinicia el juego.
* Borra los sitios y los drones, y reestablece el zoom y la ubicacion del mapa.
*/
void ResetGame(Game * game);

#endif