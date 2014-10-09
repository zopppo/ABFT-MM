#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <float.h>
#include "matrix.h"

int main(int argc, char *argv[])
{
	srand48(time(NULL) * getpid());
	/* Default values */
	int rows = 5;
	int cols = 5;
	int lower = -100, upper = 100;
	int **matrix;
	char *outFileName = "myMatrix.dat";
	int c = 0;
	int nArgs = 5;
	int argsParsed = 0;
	int mFlag = 0, nFlag = 0, lFlag = 0, uFlag = 0, oFlag = 0;


	while ((c = getopt(argc, argv, "m:n:l:u:o:")) != -1) {
		switch (c) {
			case 'm':
				rows = atoi(optarg);
				mFlag = 1;
				argsParsed++;
				break;
			case 'n':
				cols = atoi(optarg);
				nFlag = 1;
				argsParsed++;
				break;
			case 'l':
				lower = atoi(optarg);
				lFlag = 1;
				argsParsed++;
				break;
			case 'u':
				upper = atoi(optarg);
				uFlag = 1;
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
		errExit("Usage: makeMatrix -m <rows> -n <cols> -l <lower> -u <upper> -o <output filename>");
	}
	else if (argsParsed < nArgs) {
		/* Print any default values */
		if (!mFlag) {
			printf("-m %d ", rows);
		}

		if (!nFlag) {
			printf("-n %d ", cols);
		}

		if (!lFlag) {
			printf("-l %d ", lower);

		}

		if (!uFlag) {
			printf("-u %d ", upper);
		}

		if (!oFlag) {
			printf("-o %s ", outFileName);
		}

		printf("\n");
	}

	if (rows < 1)
		errExit("Column dimension must be greater than 1.");
	if (cols < 1)
		errExit("Column dimension must be greater than 1.");

	allocateMatrix(rows, cols, &matrix);
	initializeMatrix(lower, upper, rows, cols, matrix);
	writeMatrix(rows, cols, matrix, outFileName);
	deallocateMatrix(matrix);
	return 0;
}
