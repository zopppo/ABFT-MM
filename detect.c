/*
	Detects errors and writes a report to file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "matrix.h"

void writeReport(int *rowE, int *colE, int rowErrors, int colErrors, char *outFileName);

int main(int argc, char *argv[])
{
	char *inFileName = "inMatrix.dat";
	char *outFileName = "errorReport.txt";
	double **matrix;
	int rows, cols;
	int *rowE, *colE;
	int rowErrors, colErrors;
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

	if (!argsParsed) {
		errExit("Usage: -i <input filename> -o <output filename>");
	}
	else if (argsParsed < nArgs) {
		/* Print any default values */
		if (!iFlag) {
			printf("-i %s ", inFileName);
		}

		if (!oFlag) {
			printf("-o %s ", outFileName);	
		}

		printf("\n");
	}
	
	readMatrix(&rows, &cols, &matrix, inFileName);
	detect(rows, cols, matrix, &rowE, &colE, &rowErrors, &colErrors);
	deallocateMatrix(matrix);
	free(rowE);
	free(colE);
	return 0;
}

void writeReport(int *rowE, int *colE, int rowErrors, int colErrors, char *outFileName) {
	if (rowE == NULL)
		errExit("rowE is NULL. Cannot writeReport.");
	if (colE == NULL)
		errExit("colE is NULL. Cannot writeReport.");
	if (outFileName == NULL)
		errExit("Filename is NULL. Cannot writeReport");
	FILE *file = fopen(outFileName, "w");
	fprintf(file, "ERROR REPORT\n");
	printErrors(rowE, colE, rowErrors, colErrors, file);
	
}