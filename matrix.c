#include "matrix.h"
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

//#define SAFE_RECOMPUTE

int dotProductCalled = 0;

/* allocates a row-major 2D array using contiguous data block */
void allocateMatrix(int rows, int cols, int ***matrix) {
    int i;

    if (rows <= 0) {
        errExit("Rows must be positive. Cannot allocate.");
    }
    else if (cols <= 0) {
        errExit("Cols must be positive. Cannot allocate.");
    }

    if (matrix == NULL) {
        errExit("Matrix is NULL. Cannot allocate.");
    }

    *matrix = malloc(rows * sizeof(int *));
    (*matrix)[0] = malloc(rows * cols * sizeof(int));

    for (i = 1; i < rows; i++)  {
        (*matrix)[i] = (*matrix)[0] + i * cols;
    }
}

/* deallocate matrix allocated with allocateMatrix() */
void deallocateMatrix(int **matrix){
    if (matrix == NULL)
        errExit("Matrix not allocated.");
    free(matrix[0]);
    free(matrix);
}

/* initialize an allocated matrix with values distributed UNIF(lower, upper) */
void initializeMatrix(int lower, int upper, int rows, int cols, int **matrix){
    int i, j;

    if (rows <= 0)
        errExit("Rows must be positive. Cannot initialize.");
    else if (cols <= 0)
        errExit("Cols must be positive. Cannot initialize.");

    if (matrix == NULL)
        errExit("Passed NULL matrix. Cannot initialize.");

    for (i = 0; i < rows; i++)  {
        srand((int)time(NULL) + i);
        for (j = 0; j < cols; j++) {
            matrix[i][j] = rand() % upper;
            if (upper == INT_MAX)
                matrix[i][j] *= -1;
        }
    }
}

/* writes matrix to binary file, first two ints in file represent the rows and cols */
void writeMatrix(int rows, int cols, int** matrix, char* fileName) {
    if (fileName == NULL)
        errExit("Filename is null. Cannot write.");
    FILE *file = fopen(fileName, "wb");

    if (matrix == NULL)
        errExit("NULL matrix. Cannot write.");

    if (file != NULL) {
        if (fwrite(&rows, sizeof(int), 1, file) == 0)
            errExit("Rows did not write.");
        if (fwrite(&cols, sizeof(int), 1, file) == 0)
            errExit("Cols did not write.");
        if (fwrite(*matrix, sizeof(int),  rows * cols, file) == 0)
            errExit("Matrix did not write.");
    }
    else {
        errExit("That is not a valid filename.");
    }

    fclose(file);
}

/* load matrix from binary file, first two ints in file represent the rows and cols */
void readMatrix(int *rows, int *cols, int ***matrix, char *fileName) {
    FILE *file = fopen(fileName, "rb");

    if (fileName == NULL) {
        errExit("Filename is null. Cannot read.");
    }
    else if (matrix == NULL) {
        errExit("NULL matrix. Cannot read.");
    }
    else if (rows == NULL) {
        errExit("Rows is NULL. Cannot read.");
    }
    else if (cols == NULL) {
        errExit("Cols is NULL. Cannot read.");
    }

    if (file != NULL) {
        if (!fread(rows, sizeof(int), 1, file)) {
            errExit("Cannot read rows");
        }
        if (!fread(cols, sizeof(int), 1, file)) {
            errExit("Cannot read cols");
        }

        allocateMatrix(*rows, *cols, matrix);
        if (!fread(**matrix, sizeof(int), *rows * *cols, file)) {
            errExit("Cannot read matrix");
        }
    }
    else {
        errExit("%s is not a valid filename.", fileName);
    }
    fclose(file);
}

/* prints matrix to stdout */
void printMatrix(int rows, int cols, int** matrix) {
    int i, j;

    if (matrix == NULL) {
        errExit("Matrix does not exist.");
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/* multiply matrices A and B, creates new C, and places result in C */
void multiplyMatrix(int aRows, int aCols, int bRows, int bCols, int **A, int **B, int ***C) {
    int i, j;
    int cRows = aRows, cCols = bCols;
    if (A == NULL) {
        errExit("Matrix A is null.");
    }
    if (B == NULL) {
        errExit("Matrix B is null.");
    }
    if (C == NULL) {
        errExit("Matrix C is null.");
    }
    if (aCols != bRows) {
        errExit("A and B are not multipliable.");
    }
    allocateMatrix(cRows, cCols, C);
#ifdef SAFE_RECOMPUTE
    int sum, k;
    for (i = 0; i < aRows; i++) {
        for (j = 0; j < bCols; j++) {
            sum = 0;
            for (k = 0; k < aCols; k++) {
                sum += A[i][k] * B[k][j];
            }
            (*C)[i][j] = sum;
        }
    }
#else 
    for (i = 0; i < aRows; i++) {
        for (j = 0; j < bCols; j++) {
            (*C)[i][j] = dotProduct(i, aCols, bRows, j, A, B);
        }
    }
#endif
}

void oldMultiplyMatrix(int aRows, int aCols, int bRows, int bCols, int **A, int **B, int ***C) {
    int i, j, k;
    int sum;
    int cRows = aRows, cCols = bCols;
    if (A == NULL) {
        errExit("Matrix A is null.");
    }
    if (B == NULL) {
        errExit("Matrix B is null.");
    }
    if (C == NULL) {
        errExit("Matrix C is null.");
    }
    if (aCols != bRows) {
        errExit("A and B are not multipliable.");
    }
    allocateMatrix(cRows, cCols, C);

    for (i = 0; i < aRows; i++) {
        for (j = 0; j < bCols; j++) {
            sum = 0;
            for (k = 0; k < aCols; k++) {
                sum += A[i][k] * B[k][j];
            }
            (*C)[i][j] = sum;
        }
    }

}


/* Fix errors by recomputing using dot product */
bool recompute(int aRows, int aCols, int bRows, int bCols, int **A, int **B, int **C,
        int *rowE, int *colE, int rowErrors, int colErrors, int *nCorrected) {
    int i, j;
    int errorRow, errorCol;
    if (rowE[0] == -1)
        return false;
    else if (colE[0] == -1)
        return false;

    if ((rowErrors == 0 && colErrors > 0) || (colErrors == 0 && rowErrors > 0)) {
        return false;
    }
    if ((rowErrors > colErrors) || (rowErrors == colErrors)) {
        for (j = 0; j < bCols && j < colErrors; j++) {
            for (i = 0; i < aRows &&i < rowErrors; i++) {
                errorRow = rowE[i];
                errorCol = colE[j];
                /*
                   sum = 0;
                   for (k = 0; k < aCols; k++) {
                   sum += A[errorRow][k] * B[k][errorCol];
                   C[errorRow][errorCol] = sum;
                   }
                   */
                C[errorRow][errorCol] = dotProduct(errorRow, aCols, bRows, errorCol, A, B); 
                (*nCorrected)++;
            }
        }
    }
    else {
        for (i = 0; i < aRows && i < rowErrors; i++) {
            for (j = 0; j < bCols && j < colErrors; j++) {
                errorRow = rowE[i];
                errorCol = colE[j];
                /*
                   sum = 0;
                   for (k = 0; k < aCols; k++) {
                   sum += A[errorRow][k] * B[k][errorCol];
                   C[errorRow][errorCol] = sum;
                   }
                   */
                C[errorRow][errorCol] = dotProduct(errorRow, aCols, bRows, errorCol, A, B); 
                (*nCorrected)++;
            }
        }
    }
    return true;
}


int dotProduct(int aRow, int aCols, int bRows, int bCol, int **A, int **B) {
    dotProductCalled++;
    int sum = 0;
    int i;

    for (i = 0; i < aCols; i++) {
        sum += A[aRow][i] * B[i][bCol];
    }
    return sum;
}

void checkSumA(int rows, int cols, int **A, int ***Aprime) {
    int i, j;
    int sum;
    allocateMatrix(rows + 1, cols, Aprime);
    copyMatrix(rows, cols, A, *Aprime);
    if (rows <= 0) {
        errExit("Rows must be positive. Cannot checksum A.");
    }
    else if (cols <= 0) {
        errExit("Cols must be positive. Cannot checksum A.");
    }

    if (A == NULL) {
        errExit("A is null. Cannot checksum A.");
    }
    else if (Aprime == NULL) {
        errExit("Aprime is null. Cannot checksum A.");
    }


    /* Compute checksum for each column of A */
    for (j = 0; j < cols; j++) {
        sum = 0;
        for (i = 0; i < rows; i++) {
            sum += A[i][j];
        }
        (*Aprime)[rows][j] = sum;
    }
#ifdef DEBUG
    puts("SumA");
    printMatrix(rows + 1, cols, *Aprime);
#endif
}

void checkSumB(int rows, int cols, int **B, int ***Bprime) {
    int i, j;
    int sum;
    allocateMatrix(rows, cols + 1, Bprime);
    copyMatrix(rows, cols, B, *Bprime);

    if (rows <= 0)
        errExit("Rows must be positive. Cannot checksum B.");
    else if (cols <= 0)
        errExit("Cols must be positive. Cannot checksum B.");

    if (B == NULL)
        errExit("B is null. Cannot checksum B.");
    else if (Bprime == NULL)
        errExit("Bprime is null. Cannot checksum B.");

    /* Compute checksum for each row of B */
    for (i = 0; i < rows; i++) {
        sum = 0;
        for (j = 0; j < cols; j++) {
            sum += B[i][j];
        }
        (*Bprime)[i][cols] = sum;
    }
#ifdef DEBUG
    puts("SumB");
    printMatrix(rows, cols + 1, *Bprime);
#endif
}

void copyMatrix(int rows, int cols, int **sourceMatrix, int **destMatrix) {
    int i, j;
    if (sourceMatrix == NULL)
        errExit("sourceMatrix is NULL. Cannot copy.");
    else if (destMatrix == NULL)
        errExit("destMatrix is NULL. Cannot copy.");

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            destMatrix[i][j] = sourceMatrix[i][j];
        }
    }
}

/* Detects errors and prints them to a file */
bool detect(int rows, int cols, int **matrix, int **rowE, int **colE, int *rowErrors, int *colErrors) {
    int i, j;
    int check;

    if (rows <= 0)
        errExit("Rows must be positive. Cannot detect.");
    else if (cols <= 0)
        errExit("Cols must be positive. Cannot detect.");

    if (rowErrors == NULL)
        errExit("rowErrors is NULL. Cannot detect");
    if (colErrors == NULL)
        errExit("colErrors is NULL. Cannot Detect.");
    *rowErrors = 0;
    *colErrors = 0;

    if (rowE == NULL)
        errExit("rowE is NULL. Cannot detect");
    if (colE == NULL)
        errExit("colE is NULL. Cannot Detect.");
    *rowE = (int *) malloc(sizeof(int) * rows);
    *colE = (int *) malloc(sizeof(int) * cols);
    if (matrix == NULL)
        errExit("Matrix is NULL. Cannot detect.");
    /* Zero the arrays */
    for (i = 0; i < rows; i++) {
        (*rowE)[i] = -1;
    }
    for (i = 0; i < cols; i++) {
        (*colE)[i] = -1;
    }

    /* Detect row errors */
    for (i = 0; i < rows; i++) {
        check = 0;

        for (j = 0; j < cols - 1; j++) {
            check += matrix[i][j];
        }
        /* Compare with checksum value */
        if (check != matrix[i][cols - 1]) {
            (*rowE)[*rowErrors] = i;
#ifdef DEBUG
            printf("Found an error at row: %d \n", i);
#endif
            (*rowErrors)++;
        }
    }
    /* Detect column errors */

    for (j = 0; j < cols; j++) {
        check = 0;

        for (i = 0; i < rows - 1; i++) {
            check += matrix[i][j];
        }
        /* Compare with checksum value */
        if (check != matrix[rows - 1][j]) {
            (*colE)[*colErrors] = j;
#ifdef DEBUG
            printf("Found an error at column: %d \n", (*colE)[*colErrors]);
#endif
            (*colErrors)++;
        }
    }
    return (*rowErrors != 0 && *colErrors != 0);
}
/* Finds the sum of all elements in the row excluding the element at eRow and the checksum element */
int exclRowSum(int row, int col, int rows, int cols, int **matrix) {
    int i;
    int sum = 0;
    if (matrix == NULL)
        errExit("Matrix is NULL. Cannot sum.");
    if (row >= rows) {
        errExit("Error row exceeds the number of rows.");
    }
    else if (col >= cols) {
        errExit("Error column exceeds the number of columns.");
    }
    for (i = 0; i < cols - 1; i++) {
        /* if i is not the trouble column */
        if (i != col)
            sum += matrix[row][i];
    }
    return sum;
}

/* Finds the sum of all elements in the col excluding the element at eRow and the checksum element */
int exclColSum(int row, int col, int rows, int cols, int **matrix) {
    int i;
    int sum = 0;
    if (matrix == NULL)
        errExit("Matrix is NULL. Cannot sum.");
    if (row > rows) {
        errExit("Error row exceeds the number of rows.");
    }
    else if (col > cols) {
        errExit("Error column exceeds the number of columns.");
    }
    for (i = 0; i < rows - 1; i++) {
        /* if i is not the trouble row */
        if (i != row)
            sum += matrix[i][col];
    }
    return sum;
}

/* Accepts a full checksum matrix */
bool correct(int rows, int cols, int **matrix, int *rowE, int *colE, int rowErrors, int colErrors, int *nCorrected) {
    int i;
    int row, col;
    int sum;
    bool couldCorrect = true;
    *nCorrected = 0;
    if (matrix == NULL)
        errExit("Matrix is NULL. Cannot correct");
    if (rowE == NULL)
        errExit("rowE is NULL. Cannot correct");
    else if (colE == NULL)
        errExit("colE is NULL. Cannot correct.");

    /* Single error */
    if (rowErrors == 1 && colErrors == 1) {
        row = rowE[0];
        col = colE[0];
        sum = exclRowSum(row, col, rows, cols, matrix);
        /* not a checksum */
        if (col != cols - 1) {
            /* sum of row excluding the current element */

            matrix[row][col] = matrix[row][cols - 1] - sum;
        }
        else { /* fix last column */
            matrix[row][col] = sum;
        }
        (*nCorrected)++;
    }
    else if (rowErrors >= 2 && colErrors == 1) {
        col = colE[0];
        if (rowErrors > rows) {
            errExit("Row errors exceeds rows.");
        }
        /* Assumes rowE is in order */
        for (i = 0; i < rowErrors; i++) {
            row = rowE[i];
            sum = exclRowSum(row, col, rows, cols, matrix);
            /* Not checksum column */
            if (col != cols - 1) {
                /* sum of row excluding the current element */
                matrix[row][col] = matrix[row][cols - 1] - sum;
            }
            else {
                matrix[row][col] = sum;
            }
            (*nCorrected)++;
        }
    }
    else if (colErrors >= 2 && rowErrors == 1) {
        row = rowE[0];
        if (colErrors > cols) {
            errExit("Column errors exceeds columns.");
        }
        /* Assumes colE is in order */
        for (i = 0; i < colErrors; i++) {
            col = colE[i];
            /* sum of row excluding the current element */
            sum = exclColSum(row, col, rows, cols, matrix);
            if (row != rows - 1) {
                matrix[row][col] = matrix[rows - 1][col] - sum;

            }
            else {
                matrix[row][col] = sum;
            }
            (*nCorrected)++;
        }
    }
    else if (colErrors == 0 && rowErrors == 0) {
        /* We're good */
    }
    else {
        couldCorrect = false;
    }
    return couldCorrect;
}


/* Prints errors to a file */
void printErrors(int *rowE, int *colE, int rowErrors, int colErrors, FILE *file) {
    if (file == NULL)
        errExit("File is null. Cannot printErrors.");
    if (rowE == NULL)
        errExit("rowE is NULL. Cannot printErrors.");
    else if (colE == NULL)
        errExit("colE is NULL. Cannot printErrors.");
    fprintf(file, "rE: %d ", rowErrors);
    fprintf(file, "cE: %d ", colErrors);
    #ifdef SHOW_COORDS
    int i, j;
    if ((rowErrors > colErrors) || (rowErrors == colErrors)) {
        for (j = 0; j < colErrors; j++) {
            for (i = 0; i < rowErrors; i++) {
                fprintf(file, "(%d, %d) ", rowE[i], colE[j]);
            }
        }
    }
    else {
        for (i = 0; i < rowErrors; i++) {
            for (j = 0; j < colErrors; j++) {
                fprintf(file, "(%d, %d) ", rowE[i], colE[j]);
            }
        }
    }
    #endif
}


bool compareMatrix(int rows, int cols, int **A, int **B) {
    int i, j;
    bool identical = true;
    if (A == NULL)
        errExit("A is NULL. Cannot compare.");
    if (B == NULL)
        errExit("B is NULL. Cannot compare.");

    for (i = 0; i < rows && identical; i++) {
        for (j = 0; j < cols && identical; j++) {
            if (A[i][j] != B[i][j])
                identical = false;
        }
    }
    return identical;
}

/* A demonstration driver function */
void abftMultiply(int aRows, int aCols, int bRows, int bCols, int **A, int **B, int ***C) {
    /* Multipliable */
    if (aCols != bRows)
        errExit("The rows and columns do not match.");
    int **Aprime, **Bprime, **Cprime, **CprimeCopy;
    /* List of errors */
    int *rowE, *colE;
    int AprimeRows = aRows + 1, AprimeCols = aCols;
    int BprimeRows = bRows, BprimeCols = bCols + 1;
    int CprimeRows = AprimeRows, CprimeCols = BprimeCols;
    int rowErrors, colErrors;
    int nCorrected;

    /* Check sum Each Matrix */
    checkSumA(aRows, aCols, A, &Aprime);
    puts("Aprime");
    printMatrix(AprimeRows, AprimeCols, Aprime);
    checkSumB(bRows, bCols, B, &Bprime);
    puts("Bprime");
    printMatrix(BprimeRows, BprimeCols, Bprime);

    /*Multiply Matrix*/
    multiplyMatrix(AprimeRows, AprimeCols, BprimeRows, BprimeCols, Aprime, Bprime, &Cprime);
    /*Debug*/
    puts("Checksum Matrix");
    printMatrix(CprimeRows, CprimeCols, Cprime);

    allocateMatrix(CprimeRows, CprimeCols, &CprimeCopy);
    copyMatrix(CprimeRows, CprimeCols, Cprime, CprimeCopy);

    puts("After injection");
    printMatrix(CprimeRows, CprimeCols, Cprime);
    /* Look for errors using combined matrix */
    detect(CprimeRows, CprimeCols, Cprime, &rowE, &colE, &rowErrors, &colErrors);
    /* Correct errors */
    correct(CprimeRows, CprimeCols, Cprime, rowE, colE, rowErrors, colErrors, &nCorrected);
    puts("Corrected Matrix");
    printMatrix(CprimeRows, CprimeCols, Cprime);
    compareMatrix(CprimeRows, CprimeCols, Cprime, CprimeCopy);
    allocateMatrix(aRows + 1 , bCols + 1, C);
    copyMatrix(CprimeRows, CprimeCols, Cprime, *C);
}

/* Prints and error message and exits the program*/
void errExit(char *errMsg, ...) {
    va_list args;
    va_start(args, errMsg);
    vfprintf(stdout, errMsg, args);
    fprintf(stdout, "\n");
    va_end(args);
    exit(1);
}
