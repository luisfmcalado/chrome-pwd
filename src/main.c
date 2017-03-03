/*
 * File:   main.c
 * Author: Luis Calado <luisfmcalado at gmail.com>
 *
 * Created on 2 de Outubro de 2014, 14:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"

int main(int argc, char **argv) {
	
	int res = readdata();
	if(!res){
		printf("error reading data.");
		exit(1);
	}

	printf("\nPress <enter> to Quit.\n");
	int ch;
	while ((ch = getchar()) != '\n')
		;
	getchar();

	return 0;
}
