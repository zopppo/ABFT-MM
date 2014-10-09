#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "matrix.h"

int main(int argc, char *argv[])
{
	int **A, **B;
	char *aFileName = "myMatrix.dat", *bFileName = "myMatrix.dat";
	int aRows = 2, aCols = 2;
	int bRows = 2, bCols = 2;
	int nArgs = 2;
	int argsParsed = 0;
	int aFlag = 0, bFlag = 0;
	int c;

	while ((c = getopt(argc, argv, "a:b:")) != -1) {
		switch (c) {
			case 'a':
				aFileName = optarg;
				argsParsed++;
				aFlag = 1;
				break;
			case 'b':
				bFileName = optarg;
				argsParsed++;
				bFlag = 1;
				break;
		}
	}

	/* If we parsed no arguments, print usage */
	if (!argsParsed) {
		errExit("Usage: compareMatrices -a <matrix A filename> -b <matrix B filename");
	}
	else if (argsParsed < nArgs) {
		/* Print any default values */
		if (!aFlag) {
			printf("-a %s ", aFileName);
		}
		if (!bFlag) {
			printf("-b %s ", bFileName);
		}

		printf("\n");
	}
	if (aRows != bRows || aCols != bCols) {
		errExit("The matrices do not have the same dimensions.");
	}
	readMatrix(&aRows, &aCols, &A, aFileName);
	readMatrix(&bRows, &bCols, &B, bFileName);

	if (compareMatrices(aRows, aCols, A, B))
		puts("The matrices are the same.");
	else
		puts("The matrices are not the same.");

	deallocateMatrix(A);
	deallocateMatrix(B);
	return 0;
}