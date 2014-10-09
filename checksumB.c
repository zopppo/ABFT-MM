#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "matrix.h"

int main(int argc, char *argv[])
{
	int **B, **Bprime;
	char *inFileName;
	int bRows, bCols;
	char *outFileName = "outMatrix.dat";
	int c = 0;
	int nArgs = 2;
	int argsParsed = 0;
	int iFlag = 0, oFlag = 0;

	while((c = getopt(argc, argv, "b:o:")) != -1) {
		switch (c) {
			case 'b':
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
		errExit("Usage: checkSumB -b <in matrix filename> -o <output matrix filename>");
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

	readMatrix(&bRows, &bCols, &B, inFileName);
	checkSumB(bRows, bCols, B, &Bprime);
	writeMatrix(bRows, bCols + 1, Bprime, outFileName);
	deallocateMatrix(B);
	deallocateMatrix(Bprime);
	return 0;
}