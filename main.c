#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <errno.h>

#include "count_utils.h"
#include "math_utils.h"
#include "find_utils.h"
#include "allocation_utils.h"
#include "io_utils.h"


void buildLChain(struct matrix_t *matrix, struct cell_t stroke);

void getSelection(struct matrix_t *matrix, const int *columnsStars) {
    int columnIndexLeft = 0;
    int columnIndexRight = matrix->columns - 1;

    while (columnIndexLeft < columnIndexRight) {
        if (columnsStars[columnIndexLeft] == 1) {
            subtractInColumn(matrix->selection, 1, matrix->rows, columnIndexLeft);
        }

        if (columnsStars[columnIndexRight] == 1) {
            subtractInColumn(matrix->selection, 1, matrix->rows, columnIndexRight);
        }

        ++columnIndexLeft;
        --columnIndexRight;
    }
}


struct cell_t findStarInRow(const struct matrix_t *matrix, int rowIndex) {
    struct cell_t star = {-1, -1};

    for (int columnIndex = 0; columnIndex < matrix->columns; ++columnIndex) {
        if (matrix->stars[rowIndex][columnIndex] == 1) {
            star.columnIndex = columnIndex;
            star.rowIndex = rowIndex;
        }
    }
    return star;
}

struct cell_t findStar(struct matrix_t *matrix, int *selectedRows, int *selectedColumns) {
    struct cell_t stroke = {-1, -1};
    while (true) {
        stroke = findStroke(matrix);
        print2dIntArray(matrix->matrix, matrix->rows, matrix->columns, "Matrix before h-find branch");

        if (stroke.rowIndex == -1) {
            int localMinimum = findLocalMinimum(matrix);

            printIntArray(selectedColumns, matrix->columns, "Selected columns");
            for (int columnIndex = 0; columnIndex < matrix->columns; ++columnIndex) {

                if (selectedColumns[columnIndex]) {
                    subtractInColumn(matrix->matrix, localMinimum, matrix->rows, columnIndex);
                }
            }

            printIntArray(selectedRows, matrix->rows, "Selected rows");

            for (int rowIndex = 0; rowIndex < matrix->rows; ++rowIndex) {
                if (selectedRows[rowIndex]) {
                    addInRow(matrix->matrix, localMinimum, rowIndex, matrix->columns);
                }
            }

            stroke = findStroke(matrix);
            print2dIntArray(matrix->matrix, matrix->rows, matrix->columns, "Matrix after h-find branch");
        }

        print2dIntArray(matrix->matrix, matrix->rows, matrix->columns, "MATRIX BEFORE SEG FAULT");
        matrix->strokeMatrix[stroke.rowIndex][stroke.columnIndex] = 1;

        struct cell_t star = findStarInRow(matrix, stroke.rowIndex);

        if (star.rowIndex == -1) {
            break;
        } else {
            subtractInColumn(matrix->matrix, 1, matrix->rows, star.columnIndex);
            selectedColumns[star.columnIndex] = 0;

            addInRow(matrix->matrix, 1, star.rowIndex, matrix->columns);
            selectedRows[star.rowIndex] = 1;
        }

        printf("Stars matrix: \n");
        printBoolArray(matrix->stars, matrix->rows, matrix->columns);
        printf("----\n");
    }

    return stroke;
}

void calculateStarsMatrix(struct matrix_t *matrix) {
    int starsAmount = findStars(matrix);

    while (starsAmount < matrix->columns) {
        int *selectedRows = (int *) calloc(matrix->rows, sizeof(int));
        matrix->selection = (int **) allocate(sizeof(int), matrix->rows, matrix->columns);
        matrix->strokeMatrix = (int **) allocate(sizeof(int), matrix->rows, matrix->columns);

        int *selectedColumns = countColumnsStars(matrix);
        getSelection(matrix, selectedColumns);

        struct cell_t stroke = {-1, -1};

        stroke = findStar(matrix, selectedRows, selectedColumns);

        buildLChain(matrix, stroke);

        starsAmount = countStars(matrix);

        free(selectedColumns);
        free(selectedRows);
        free2d((void **) matrix->selection, matrix->rows);
        free2d((void **) matrix->strokeMatrix, matrix->rows);
    }
}

void buildLChain(struct matrix_t *matrix, struct cell_t stroke) {
    int rowIndex = stroke.rowIndex;
    int columnIndex = stroke.columnIndex;

    while (rowIndex > 0 && rowIndex < matrix->rows
           && columnIndex > 0 && columnIndex < matrix->columns) {
        matrix->strokeMatrix[rowIndex][columnIndex] = 0;
        matrix->stars[rowIndex][columnIndex] = 1;

        int rowIterator = 0;
        while (rowIterator <= matrix->rows
               && (matrix->stars[rowIterator][columnIndex] != 1 || rowIterator == rowIndex)) {
            ++rowIterator;
        }

        if (rowIterator <= matrix->rows) {
            int columnIterator = 0;
            while (columnIterator < matrix->columns
                   && (matrix->strokeMatrix[rowIterator][columnIterator] != 1 || columnIterator == columnIndex)) {
                ++columnIterator;
            }

            if (columnIterator < matrix->columns) {
                matrix->stars[rowIndex][columnIndex] = 0;
            }

            columnIndex = columnIterator;
        }

        rowIndex = rowIterator;
    }
}


int getFinalResult(struct matrix_t *matrix) {
    int result = 0;

    for (int rowIndex = 0; rowIndex < matrix->rows; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < matrix->columns; ++columnIndex) {
            if (matrix->stars[rowIndex][columnIndex]) {
                result += matrix->startMatrix[rowIndex][columnIndex];
            }
        }
    }

    return result;
}

int findMinimum(struct matrix_t *matrix) {
    matrix->stars = (bool **) allocate(sizeof(bool), matrix->rows, matrix->columns);

    if (matrix->stars == NULL) {
        printf("Exit from findMinimum. Unable to allocate memory for stars");
        return -1;
    }

    int *columnMinimums = findColumnMinimums(matrix);
    if (columnMinimums == NULL) {
        printf("Exit from findMinimum. Null from findColumnMinimums");
        return -1;
    }

    subtractColumnMinimum(matrix, columnMinimums);

    int *rowMinimums = findRowMinimums(matrix);
    if (rowMinimums == NULL) {
        printf("Exit from findMinimum. Null from rowMinimums");
        return -1;
    }

    subtractRowMinimum(matrix, rowMinimums);

    calculateStarsMatrix(matrix);

    return getFinalResult(matrix);
}


int main() {
    struct matrix_t *matrix = inputData("../input.txt");
    if (matrix == NULL) {
        printf("\n%s", strerror(errno));
        return 0;
    }

    print2dIntArray(matrix->matrix, matrix->rows, matrix->columns, "INITIAL MATRIX");

    int minimum = findMinimum(matrix);
    printf("\n\nGlobal minimum = %d", minimum);

    return 0;
}
