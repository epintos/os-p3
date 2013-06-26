#include "../../include/iolib.h"
#include "../../include/string.h"
#include "../../include/process.h"
#include "../../include/systemCalls.h"
#include "../../include/filesystem2.h"
#include "../../include/diskata.h"
#include "../../include/malloc.h"
#include "../../include/defs.h"
#include "../../include/AtaDiskAdapter.h"
#include "../../include/users.h"

int procA(int argc, char ** argv) {

	//		generalHeader* header = Malloc(sizeof(generalHeader));
	//	fragSector sect;

	printf("Se quiere crear el directorio hola3\n");
	if (mkdir("hola3") == -1) {
		printf("Esto no deberia pasar \n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf("Se quiere crear el archivo myar.txt en el directorio actual\n");
	if (creat("myar.txt", 511) == -1) {
		printf("Error en myar.txt\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf("Se crea el directorio jorge, hijo del directorio actual\n");
	if (mkdir("jorge") == -1) {
		printf("Error en Jorge\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf("Se crea el directorio jorgito, hijo del directorio actual\n");
	if (mkdir("jorgito") == -1) {
		printf("Error en Jorgito\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf("Se crea el directorio chau dentro de jorgito\n");
	if (mkdir("jorgito/chau") == -1) {
		printf("Errot en Chau\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf("Se crea el directorio nestor, hijo del directorio jorgito/chau\n");
	if (mkdir("jorgito/chau/nestor") == -1) {
		printf("Error en Jorge\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf("Se crea el directorio jorgito1, hijo del directorio actual\n");
	if (mkdir("jorgito1") == -1) {
		printf("Error en Jorge\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf("Se crea el directorio jorgito2, hijo del directorio actual\n");
	if (mkdir("jorgito2") == -1) {
		printf("Error en Jorge\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf("Se crea el archivo myar.txt en jorgito/chau/nestor\n");
	if (creat("jorgito/chau/nestor/myar.txt", 511) == -1) {
		printf(" Esto no deberia pasar\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}

	printf(
			"Se abre el archivo myarchivo.txt para lectura solamente en jorgito/chau/nestor\n");
	if (open("jorgito/chau/nestor/myarchivo.txt", O_RDONLY) == -1) {
		printf("Error esperado. OK\n");
	} else {
		printf("Creado satisfactoriamente\n\n");
	}
//
//	printf("Se crea un link del archivo users.txt\n");
//	if (link("/dinux/users/users.txt", "newlink") == -1)
//		printf("ERROR LINK\n");
//	else {
//		printf("El Link se creo exitosamente\n");
//		//
//		//		//		diskRead((char*) header, sizeof(generalHeader), 72,
//		//		//				sizeof(generalHeader) * 4);
//		//		//		printHeader(header);
//		//		//
//		//		//		diskRead((char*) &sect, SECTOR_SIZE, header->dataSect, 0);
//		//		//		printf("%s\n", sect.info);
//		//		//
//		//		//		diskRead((char*) header, sizeof(generalHeader), 72,
//		//		//				sizeof(generalHeader) * 3);
//		//		//		printHeader(header);
//		//		//
//		//		//		diskRead((char*) &sect, SECTOR_SIZE, header->dataSect, 0);
//		//		//		printf("%s\n", sect.info);
//		//
//	}
//
//	printf("Se mueve newlink a dinux\n");
//	mv("newlink", "dinux");
//	//
//	printf("Se mueve la carpeta jorgito a dinux");
//	mv("jorgito", "dinux");
//
//	printf("Se copia Users.txt a donde estoy parado\n");
//	cp("dinux/users/users.txt", ".");
//
//	printf("Se copia newLink a donde estoy parado\n");
//	cp("dinux/newlink", ".");
//
//	cp("newlink", "jorge");
//
//	mv("jorge", "jorgito1");

	return 0;

}

