#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "matrix.h"
#include <sys/time.h>
#include <time.h>

int ticksToMs(clock_t *tickTime);

int toMS(struct timeval* tv) {

	return ((tv->tv_usec)/1000 + tv->tv_sec*1000);

}


int main(int argc, char *argv[])
{
	int **A, **B, **Aprime, **Bprime, **CprimeG, **Cprime;
	char *aFileName = "A.dat", *bFileName = "B.dat", *cPrimeFileName = "CPrime.dat";
	int aRows = 5, aCols = 5;
	int bRows = 5, bCols = 5;
	int cRows = aRows, cCols = bCols;
	int *rowE, *colE;
	int rowErrors, colErrors;
	int nCorrected = 0;
	int nRecomputed = 0;
	int nRecomputeCalled = 0;
	int c = 0;
	int nArgs = 3;
	int argsParsed = 0;
	int aFlag = 0, bFlag = 0, cFlag = 0;
	bool isCorrect;

	while ((c = getopt(argc, argv, "a:b:c:")) != -1) {
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
			case 'c':
				cPrimeFileName = optarg;
				argsParsed++;
				cFlag = 1;
				break;
		}
	}

	/* If we parsed no arguments, print usage */
	if (!argsParsed) {
		errExit("Usage: abtfMultiply -a <matrix A filename> -b <matrix B filename> -c <matrix CPrime filename>");
	}
	else if (argsParsed < nArgs) {
		/* Print any default values */
		if (!aFlag) {
			printf("-a %s ", aFileName);
		}
		if (!bFlag) {
			printf("-b %s ", bFileName);
		}
 		if (!cFlag) {
			printf("-c %s ", cPrimeFileName);
		}
		printf("\n");
	}

	readMatrix(&aRows, &aCols, &A, aFileName);
	readMatrix(&bRows, &bCols, &B, bFileName);
	readMatrix(&cRows, &cCols, &CprimeG, cPrimeFileName);

	checkSumA(aRows, aCols, A, &Aprime);

	checkSumB(bRows, bCols, B, &Bprime);

	multiplyMatrix(aRows + 1, aCols, bRows, bCols + 1, Aprime, Bprime, &Cprime);

	detect(aRows + 1, bCols + 1, Cprime, &rowE, &colE, &rowErrors, &colErrors);

	isCorrect = correct(aRows + 1, bCols + 1, Cprime, rowE, colE, rowErrors, colErrors, &nCorrected);

 	if (!isCorrect) {
		recompute(aRows + 1, aCols, bRows, bCols + 1, Aprime, Bprime, Cprime, rowE, colE, rowErrors, colErrors, &nRecomputed);
		nRecomputeCalled++;
 	}


	printErrors(rowE, colE, rowErrors, colErrors, stdout);
	
	printf("Corrected: %d ", nCorrected);
	printf("Recomputed: %d ", nRecomputed);
	printf("Recompute called: %d ", nRecomputeCalled);
	if (compareMatrix(aRows + 1, bCols + 1, Cprime, CprimeG) == true){
		printf("Correct");
	}
	else {
		printf("Not Correct");
		writeMatrix(aRows + 1, bCols + 1, Cprime, "error.dat");
	}
	printf("\n");

	deallocateMatrix(A);
	deallocateMatrix(B);
	deallocateMatrix(Aprime);
	deallocateMatrix(Bprime);
	deallocateMatrix(CprimeG);
	deallocateMatrix(Cprime);
	return 0;
}

int ticksToMs(clock_t *tickTime) {
	int msec = (*tickTime * 1000) / CLOCKS_PER_SEC;
	return msec%1000;
}
