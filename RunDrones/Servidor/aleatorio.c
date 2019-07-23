#include "aleatorio.h"

/**
 * Generar un numero aleatorio entre 0 y 1
 * @return numero aleatorio entre 0 y 1	
*/
float aleatorio(){
    float result;
    result=(float)(rand() % ((2000-100+1) + 100));
  
  //  
    return result;
}