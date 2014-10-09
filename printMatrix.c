#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "matrix.h"

int main(int argc, char *argv[])
{
	/* no default filename */
	char *fileName;
	int c = 0;
	double **matrix;
	int rows, cols;
	int argsParsed = 0;

	while ((c = getopt(argc, argv, "i:")) != -1) {
		switch (c) {
			case 'i':
				fileName = optarg;
				argsParsed++;
				break;
		}
	}

	/* If we parsed no arguments, print usage. */
	if (!argsParsed) {
		errExit("Usage: printMatrix -i <matrix filename>");
	}

	readMatrix(&rows, &cols, &matrix, fileName);
	printMatrix(rows, cols, matrix);	
	deallocateMatrix(matrix);
	return 0;
} 