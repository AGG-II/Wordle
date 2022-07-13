#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef __INCLUDE_THIS__
#define __INCLUDE_THIS__
#include "constantes.h"
#include "funciones.h"
#endif

void getAmountOfPlays(int* input) {

	printf("Ingrese la cantidad de veces que va a jugar (MAX 8 veces):\n");
	do{
	printf(">");
	scanf("%d",input);
	}while(*input <  0 || *input > CANTIDAD_DE_PARTIDAS_MAX );
	getchar();
	return;
}

//esta funcion recibe una lista de posiciones prohibidas que ya aparecieron y devuelve un numero que no pertenezca
//a la lista y que este dentro de la cantidad de palabras disponibles
void elegir_palabra(int prohibidas[], char* palabra, int *Pjugadas){
        int pos = GNUM(), i;
	for(i=0;i < (*Pjugadas);i++) {
		//si la lista contiene a ese numero, genera uno nuevo y revisa de nuevo	
		if(pos == prohibidas[i]) {//Esta linea tira un segmentation fault (por entrar a donde no debe probablemente)
			pos = GNUM();
			i = 0;
		}
	}
	prohibidas[i]= pos;//la nueva posicion se agrega a las prohibidas
	getWordInLine(NOMBRE_DE_ARCHIVO, pos, palabra);
	return ;	
}

//esta funcion modifica el string evaluacion para devolver una linea del mapa de la partida y calcular el puntaje actual
int evaluar(Resultado* resultadoP, char* intento, char* evaluacion, char* comoVa) {

	int i, j;
	for(i = 0; i < LONGITUD_DE_LAS_PALABRAS; i++){
		
		evaluacion[i] = '_';//primero asumimos que la letra no es correcta
		
		for(j = 0; j < LONGITUD_DE_LAS_PALABRAS; j++) {
			if(((resultadoP -> palabra)[j]) == *(intento + i)) {//si la letra esta en la palabra
				if(i == j){//si esta bien colocada
					if(*(comoVa + j) != 'v') {//si hasta ahora no salio en la posicion correcta
						*(comoVa + j) = 'v';
						resultadoP-> puntaje += 100;
					}
					evaluacion[i] = 'v';
				}else {//si no esta bien colocada
					if(*(comoVa + j) == '_') {//si hasta ahora no habia salido
						*(comoVa + j) = '-';
						resultadoP-> puntaje += 50;
					}
					evaluacion[i] = '-';
				}
			}
		}
	}
	evaluacion[i] = '\0';
	return strcmp(evaluacion , "vvvvv");//se que lo ideal seria comparar la palabra con el intento pero por el momento eso no funciona
}

void acerto(char* evaluacion, Resultado* partida, int NroIntentos) {

	printf("Acerto!!!\n");
	partida -> puntaje += PUNTOS_POR_ACERTAR;
	partida -> acerto = 1;
	if(NroIntentos == 0){//acerto al primer intento?
		partida -> puntaje = PUNTAJE_PERFECTO;
	}
	return;
}

//en esta parte es cuando comienza la partida en si, toma un intento y devuelve un string que marca las posiciones correctas
void  intentar(Resultado * resultadoP) {

	char intento[CANTIDAD_DE_INTENTOS_MAXIMA][LONGITUD_DE_LAS_PALABRAS + 1];
	int i, quizasAcerto;
	char evaluacion[LONGITUD_DE_LAS_PALABRAS + 1],comoVa[LONGITUD_DE_LAS_PALABRAS];
	for(i = 0; i <= LONGITUD_DE_LAS_PALABRAS; i++) comoVa[i] = '_';//llenamos todos los espacios del array
	resultadoP -> puntaje = PUNTAJE_INICIAL;//inicializamos el puntaje
	
	for(i = 0; i < CANTIDAD_DE_INTENTOS_MAXIMA; i++){
	printf(">");
	scanf("%s", intento[i]);
	mayusculizar(intento[i]);
	quizasAcerto = evaluar(resultadoP, intento[i], evaluacion,  comoVa);
	printf(">%s\n", evaluacion);
	
	if(!quizasAcerto) {
	acerto(evaluacion, resultadoP, i);
	return;
	}
	resultadoP -> puntaje -= NO_ACERTO;
	}
	resultadoP -> acerto = 0;//no acerto la palabra
	return;
}

void jugar(Resultado* resultadoP, int *jugadas, int partidas) { 
	*jugadas = *jugadas + 1; 
	printf("\nLa palabra es :%s\n",resultadoP -> palabra);
	printf("Partida numero: %d de %d \n",(*jugadas), partidas);
	intentar(resultadoP);
	if(*jugadas != partidas) consultar()? *jugadas:(*jugadas = ((*jugadas) + partidas));
	return;

}


//preguntamos si la persona quiere seguir jugando
int  consultar() {
	char resp,c;
	getchar();
	printf("Seguir jugando?(Y/N)\n");
	do{
	printf(">");
	scanf("%c", &resp);
	while((c = getchar())!= '\n');
	if(resp == 'y' || resp == 'Y') {
		return 1;
	}
	}while(!(resp == 'N' || resp == 'n' ));
	return 0;
}

void mostrarCadaPartida(Resultado* partidas, int partidasJ) {

	int i, prom = 0, Caciertos = 0;
	int Pmax = (PUNTAJE_INICIAL - (CANTIDAD_DE_INTENTOS_MAXIMA * NO_ACERTO)),Pmin = PUNTAJE_PERFECTO;
	borrar_pantalla();
	for(i = 0; i < partidasJ; i++) {//mostrar hasta que se hayan mostrado todas las partidas
		
		printf("Partida Nro %i\nPalabra:%s\nPuntaje:%i\n\n",i+1,(partidas + i) -> palabra,(partidas + i) -> puntaje);
		if((partidas + i) -> puntaje > Pmax) Pmax = (partidas + i) -> puntaje;
		if((partidas + i) -> puntaje < Pmin) Pmin = (partidas + i) -> puntaje;
		if((partidas + i) -> acerto) {
		prom += (partidas + i) -> puntaje;
		Caciertos++;
		}
	}
	printf("\nPresione ENTER para continuar...");
	while(getchar()!= '\n');
	borrar_pantalla();	
	printf("La/s mejor/es partida/s:\n");
	for(i = 0; i < partidasJ; i++)
		if((partidas + i) -> puntaje == Pmax)
		printf("Partida Nro %i\nPalabra:%s\nPuntaje:%i\n\n",i+1,(partidas + i) -> palabra,(partidas + i) -> puntaje);
	
	printf("La/s peor/es partida/s:\n");
	for(i = 0; i < partidasJ; i++)
		if((partidas + i) -> puntaje == Pmin)
		printf("Partida Nro %i\nPalabra:%s\nPuntaje:%i\n\n",i+1,(partidas + i) -> palabra,(partidas + i) -> puntaje);
	printf("El puntaje promedio de las partidas ganadas es %.2f\n",(float)prom/Caciertos);
	printf("Presione ENTER para finalizar.\n");
	while(getchar() != '\n');
	return;
}

//Para evitar que marque como error un intento donde todas las letras son minusiculas
void mayusculizar(char* input) {

	int i;
	for(i = 0; *(input + i) != '\0'; i++) {
	
		*(input + i) = toupper(*(input + i));
	}
	
	return;

}

void getWordInLine(char *fileName, int lineNumber, char *p) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(fileName, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int i=1;
    while ((read = getline(&line, &len, fp)) != -1) {
        if (i==lineNumber) {
           strcpy(p, line);
	   *(p + (LONGITUD_DE_LAS_PALABRAS + 1)) = '\0';//el ultimo caracter lo reemplaza con un terminador
           return;
        }
        i++;
    }
}

//inciar sesion recibe la cantidad de veces que se va a jugar
void iniciar_sesion(int partidas) {
     printf("Se van a jugar un total de %d partidas.\n", partidas);
     int palabrasP[CANTIDAD_DE_PARTIDAS_MAX];//posiciones de palabras que ya salieron
     int  Pjugadas = 0;//cantidad de partidas jugadas
     Resultado RePartidas[CANTIDAD_DE_PARTIDAS_MAX];//Recoleccion de todas las Partidas
     while(Pjugadas < partidas){
	elegir_palabra(palabrasP, ((RePartidas + Pjugadas) -> palabra), &Pjugadas);
	jugar((RePartidas + Pjugadas), &Pjugadas, partidas);
     }
     Pjugadas -= partidas;//esta resta no da 0 si se decidio terminar antes la sesion
     if(!Pjugadas) Pjugadas = partidas; //si la resta anterior da 0 entonces se jugaron todas las partidas esperadas
     mostrarCadaPartida(RePartidas , Pjugadas);
     //free(palabrasP);
     return;
}

int main() {
     srand(gsemilla());
     int *cant = malloc(sizeof(int));
     // acordate de liberar la memoria y de borrar esta linea cuando termines con el programa
     // (o dejala a modo de chiste cuando le muestres el codigo al profesor)
     
     getAmountOfPlays(cant);//pedimos la cantidad de veces que se va a jugar
     borrar_pantalla();
     iniciar_sesion(*cant);
     free(cant);
     return 0;
}
