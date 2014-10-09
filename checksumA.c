#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "matrix.h"

int main(int argc, char *argv[])
{
	double **A, **Aprime;
	char *inFileName;
	int aRows, aCols;
	char *outFileName = "outMatrix.dat";
	int c = 0;
	int nArgs = 2;
	int argsParsed = 0;
	int iFlag = 0, oFlag = 0;

	while((c = getopt(argc, argv, "a:o:")) != -1) {
		switch (c) {
			case 'a':
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
		errExit("Usage: checkSumA -a <in matrix filename> -o <output matrix filename>");
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

	readMatrix(&aRows, &aCols, &A, inFileName);
	checkSumA(aRows, aCols, A, &Aprime);
	writeMatrix(aRows + 1, aCols, Aprime, outFileName);
	deallocateMatrix(A);
	deallocateMatrix(Aprime);
	return 0;
}