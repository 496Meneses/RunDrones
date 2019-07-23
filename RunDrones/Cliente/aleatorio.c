#include "aleatorio.h"


/**
 * Generar un numero aleatorio entre 0 y 1
 * @return numero aleatorio entre 0 y 1	
*/
float aleatorio(){
    float result;
    result=(float)(rand() % RANGO);
  //  
    return result;
}