#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "matrix.h"

int main(int argc, char *argv[])
{
	char *inFileName;
	char *outFileName = "outMatrix.dat";
	int **matrix;
	int *rowE, *colE;
	int rowErrors, colErrors;
	int nCorrected;
	int rows, cols;
	int c = 0;
	int nArgs = 2;
	int argsParsed = 0;
	int iFlag = 0, oFlag = 0;

	while ((c = getopt(argc, argv, "i:o:")) != -1) {
		switch (c) {
			case 'i':
				inFileName = optarg;
				iFlag = 1;
				argsParsed++;
				break;
			case 'o':
				outFileName = optarg;
				oFlag = 1;
				argsParsed++;
				break;
		}
	}
	/* If we parsed no arguments, print usage */
	if (!argsParsed) {
		errExit("Usage: correct -i <in matrix filename> -o <output filename>");
	}
	else if (!iFlag) {
		errExit("Must enter input matrix filename.");
	}
	else if (argsParsed < nArgs) {
		/* Print any default values */
		if (!oFlag) {
			printf("-o %s ", outFileName);
		}

		printf("\n");
	}

	readMatrix(&rows, &cols, &matrix, inFileName);
	detect(rows, cols, matrix, &rowE, &colE, &rowErrors, &colErrors);
	correct(rows, cols, matrix, rowE, colE, rowErrors, colErrors, &nCorrected);

	deallocateMatrix(matrix);
	free(rowE);
	free(colE);
	return 0;
}