#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "matrix.c"

int main(int argc, char *argv[])
{
	int **A, **B, **C;
	char *aFileName = "myMatrix.dat", *bFileName = "myMatrix.dat", *cFileName = "myMultMatrix.dat";
	int aRows = 2, aCols = 2;
	int bRows = 2, bCols = 2;
	int c = 0;
	int nArgs = 3;
	int argsParsed = 0;
	int aFlag = 0, bFlag = 0, oFlag = 0;

	while ((c = getopt(argc, argv, "a:b:o:")) != -1) {
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
			case 'o':
				cFileName = optarg;
				argsParsed++;
				oFlag = 1;
				break;
		}
	}

	/* If we parsed no arguments, print usage */
	if (!argsParsed) {
		errExit("Usage: multiplyMatrix -a <matrix A filename> -b <matrix B filename -o <output filename>");
	}
	else if (argsParsed < nArgs) {
		/* Print any default values */
		if (!aFlag) {
			printf("-a %s ", aFileName);
		}
		if (!bFlag) {
			printf("-b %s ", bFileName);
		}

		if (!oFlag) {
			printf("-o %s ", cFileName);
		}

		printf("\n");
	}

	readMatrix(&aRows, &aCols, &A, aFileName);
	readMatrix(&bRows, &bCols, &B, bFileName);

	multiplyMatrix(aRows, aCols, bRows, bCols, A, B, &C);
	writeMatrix(aRows, bCols, C, cFileName);
	deallocateMatrix(A);
	deallocateMatrix(B);
	deallocateMatrix(C);
	return 0;
}