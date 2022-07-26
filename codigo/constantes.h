#define NOMBRE_DE_ARCHIVO "palabras.txt"
#define CANTIDAD_DE_PALABRAS 30
#define CANTIDAD_DE_PARTIDAS_MAX 8
#define CANTIDAD_DE_INTENTOS_MAXIMA 5
#define LONGITUD_DE_LAS_PALABRAS 5
#define PUNTAJE_INICIAL 5000
#define NO_ACERTO 500
#define PUNTOS_POR_ACERTAR 2000
#define PUNTAJE_PERFECTO 10000

//esta macro permite determinar si el sistema operativo tiene acceso a la libreria unistd.h y por lo tanto tiene acceso
//a la funcion getpid() y si para poder limpiar la pantalla es necesario utilizar 'cls' en vez de 'clear'
#if defined(_WIN32) || defined(_WIN64)
	#include <time.h>
	#define borrar_pantalla() system("cls")
	#define gsemilla() time(NULL)
#else
	#include <unistd.h>
	#define borrar_pantalla() system("clear")
	#define gsemilla() getpid()
#endif

//esta macro genera un numero aleatorio teniendo en cuenta la cantidad de palabras que hay en el archivo
#define GNUM() (rand() % CANTIDAD_DE_PALABRAS) + 1

typedef struct {

	char palabra[LONGITUD_DE_LAS_PALABRAS + 1];
	int puntaje;
	int acerto;
}Resultado;
