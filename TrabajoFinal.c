#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOMBRE_DE_ARCHIVO  "palabras.txt"
#define CANTIDAD_DE_PALABRAS 30 
#define CANTIDAD_DE_PARTIDAS_MAX 8
#define CANTIDAD_DE_INTENTOS_MAXIMA 5
#define LONGITUD_DE_LAS_PALABRAS 5

//esta macro permite determinar si el sistema operativo tiene acceso a la libreria unistd.h y por lo tanto tiene acceso
//a la funcion getpid() y si para  poder limpiar la pantalla es necesario utilizar 'cls' en vez de 'clear'
#if defined(__CYGWIN__ ) || defined(_WIN32) || defined(_WIN64)
	 #include <time.h>
	 #define borrar_pantalla() system("cls")
	 #define gsemilla() time(NULL)
#else
	 #include <unistd.h>
	 #define borrar_pantalla() system("clear")
	 #define gsemilla() getpid()
#endif

//esta macro genera un numero aleatorio teniendo en cuenta la cantidad de palabras que hay en el arhcivo
#define GNUM() (rand() % CANTIDAD_DE_PALABRAS) + 1

struct Resultado {

	char* palabra;
	int puntaje;
	int acerto;
};

void getWordInLine(char*, int, char*);
void getAmountOfPlays(int*);
void iniciar_sesion(int*);
void elegir_palabra(int*, char*, int*);
void jugar(char*, int*, int);
struct Resultado intentar(char*);
void evaluar(char[][LONGITUD_DE_LAS_PALABRAS+1], char*, char*, int);
int consultar();
struct Resultado puntuar(char[][LONGITUD_DE_LAS_PALABRAS + 1], int, int, char*);

int main() {
     srand(gsemilla());
     int *cant = malloc(sizeof(int));
     // acordate de liberar la memoria y de borrar esta linea cuando termines con el programa
     // (o dejala a modo de chiste cuando le muestres el codigo al profesor)
     
     getAmountOfPlays(cant);
     borrar_pantalla();
     iniciar_sesion(cant);
     free(cant);
     return 0;
}    

void getAmountOfPlays(int* input) {

	printf("Ingrese la cantidad de veces que va a jugar (MAX 8 veces):\n");
	do{
	printf(">");
	scanf("%d",input);
	}while(*input <  0 || *input > CANTIDAD_DE_PARTIDAS_MAX );
	getchar();
	return;
}

//inciar sesion recibe la cantidad de veces que se va a jugar
void iniciar_sesion(int* partidas) {
     printf("Se van a jugar un total de %d partidas.\n", *partidas);
     char palabra[6];
     int palabrasP[CANTIDAD_DE_PARTIDAS_MAX];//posiciones de palabras que ya salieron
     //int * palabrasP = (int*)malloc(sizeof(int) * (*partidas));
     int  Pjugadas = 0;//cantidad de partidas jugadas
     while(Pjugadas < (*partidas)){
	elegir_palabra(palabrasP, palabra, &Pjugadas);
	jugar(palabra, &Pjugadas, (*partidas));
     }
     //free(palabrasP);
     return;
}

//esta funcion recibe una lista de posiciones prohibidas que ya aparecieron y devuelve un numero que no pertenezca
//a la lista y que este dentro de la cantidad de palabras disponibles
void elegir_palabra(int prohibidas[], char* palabra, int *Pjugadas){
        int pos = GNUM() , i;
	for(i=0;i < (*Pjugadas);i++) {
		//si la lista contiene a ese numero, genera uno nuevo y revisa de nuevo	
		if(pos == prohibidas[i]) {//Esta linea tira un segmentation fault (por entrar a donde no debe probablemente)
			pos = GNUM();
			i = 0;
		}
	}
	prohibidas[i]= pos;
	getWordInLine(NOMBRE_DE_ARCHIVO, pos, palabra);
	return ;	
}

void jugar(char* palabra, int *jugadas, int partidas) { 
	*jugadas = *jugadas + 1; 
	printf("\nLa palabra es :%s\n",palabra);
	printf("Partida numero: %d de %d \n",(*jugadas), partidas);
	intentar(palabra);
	if(*jugadas != partidas) consultar()? *jugadas:(*jugadas = ((*jugadas) + partidas));
	return;
}

//en esta parte es cuando comienza la partida en si, toma un intento y devuelve un string que marca las posiciones correctas
struct Resultado intentar(char* palabra) {

	char intento[CANTIDAD_DE_INTENTOS_MAXIMA][LONGITUD_DE_LAS_PALABRAS + 1];
	int acerto, i;
	char evaluacion[LONGITUD_DE_LAS_PALABRAS];
	for(i = 0; i < CANTIDAD_DE_INTENTOS_MAXIMA; i++){
	printf(">");
	scanf("%s", intento[i]);
	evaluar(intento,palabra,evaluacion,i);
	printf(">%s\n", evaluacion);//imprime el resultado
	if(!(acerto = strcmp(evaluacion, "vvvvv"))){//comprendo que lo ideal seria comparar la palabra con el intento, pero por algun motivo eso no esta funcionando actualmente (a mejorar)
		printf("Acerto!!!\n");
		return puntuar(intento, i, acerto, palabra);
	}
	}
	return puntuar(intento , i, acerto, palabra);

}

void evaluar(char intento[][LONGITUD_DE_LAS_PALABRAS + 1], char* palabra, char* evaluacion, int nroI) {

	int i, j;
	for(i = 0; i < LONGITUD_DE_LAS_PALABRAS; i++){
		evaluacion[i] = '_';
		for(j = 0; j < LONGITUD_DE_LAS_PALABRAS; j++) {
			if(intento[nroI][i] == *(palabra + j)) evaluacion[i] = '-';
			if(intento[nroI][i] == *(palabra + i)) evaluacion[i] = 'v';
		}
	}
	evaluacion[i] = '\0';
	return;
}

struct Resultado puntuar(char intentos[][LONGITUD_DE_LAS_PALABRAS + 1], int cantidad, int acerto, char* palabra) {

	int i;
	struct Resultado Pperfecta;
	Pperfecta.palabra = palabra;
	Pperfecta.puntaje = 10000;
	if(!cantidad) return Pperfecta;
	int puntaje = 5000 - (cantidad * 500);// restamos la cantidad de intentos 
	puntaje = (2000 * ((acerto? 1:0)));//si acerto le sumamos el puntaje por haberlo echo
	for(i = 0; i < cantidad; i++){
	}
		
}

//preguntamos si la persona quiere seguir jugando
int  consultar() {
	char resp,c;
	getchar();
	printf("Seguir jugando?(Y/N)\n");
	do{
	printf(">");
	scanf("%c", &resp);
	if(resp == 'y' || resp == 'Y') {
		return 1;
	}
	while((c = getchar())!= '\n');
	}while(!(resp == 'N' || resp == 'n' ));
	return 0;
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
           return;
        }
        i++;
    }
}
