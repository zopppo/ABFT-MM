#ifndef MATRIX_H
#define MATRIX_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern int dotProductCalled;
typedef enum {false, true} bool;

/* allocates a row-major 2D array using contiguous data block */
void allocateMatrix(int rows, int cols, int*** matrix);

/* deallocate matrix allocated with allocateMatrix() */
void deallocateMatrix(int **matrix);

/* initialize an allocated matrix with values distributed UNIF(lower, upper) */
void initializeMatrix(int lower, int upper, int rows, int cols, int **matrix);

/* writes matrix to binary file, first two ints in file represent the rows and cols */
void writeMatrix(int rows, int cols, int **matrix, char *fileName);

/* load matrix from binary file, first two ints in file represent the rows and cols */
void readMatrix(int *rows, int *cols, int ***matrix, char *fileName);

/* Copy the contents of the source matrix to the destination matrix */
void copyMatrix(int rows, int cols, int **sourceMatrix, int **destMatrix);

/* returns true if the matrices are identical */
bool compareMatrix(int rows, int cols, int **A, int **B);

/* prints the matrix to stdout */
void printMatrix(int rows, int cols, int** matrix);

/* Prints the errors in a row and col error array*/
void printErrors(int *rowE, int *colE, int rowErrors, int colErrors, FILE *file);

/* Corrects errors found at Cij by recomputing Ai dot Bj */
void recompute(int aRows, int aCols, int bRows, int bCols, int **A, int **B, int **C,
	int *rowE, int *colE, int rowErrors, int colErrors, int *nCorrected);

/* multiply matrices A and B, creates new C, and places result in C */
void multiplyMatrix(int aRows, int aCols, int bRows, int bCols, int** A, int** B, int*** C);

/* does a fault tolerant multiplication of two matrices and places the result in C.*/
void abftMultiply(int aRows, int aCols, int bRows, int bCols, int **A, int **B, int ***C);

/* Appends an additional checksum row to A and places it within Aprime*/
void checkSumA(int rows, int cols, int **A, int ***Aprime);

/* Appends an additional checksum column to B and places it within Bprime*/
void checkSumB(int rows, int cols, int **B, int ***Bprime);

/* Looks for errors in the matrix and places the results into the row error and column error arrays
	The error arrays have -1 sentinel values.
*/
bool detect(int rows, int cols, int **matrix, int **rowE, int **colE, int *rowErrors, int *colErrors);

/* Correct errors in the matrix */
bool correct(int rows, int cols, int **matrix, int *rowE, int *colE, int rowErrors, int colErrors, int *nCorrected);

int dotProduct(int aRow, int aCols, int bRows, int bCol, int **A, int **B);

/* Prints and error message and exits. */
void errExit(char *errMsg, ...);
#endif
