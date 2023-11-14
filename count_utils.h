//
// Created by avmd on 11.11.23.
//

#ifndef PPP_COUNT_UTILS_H
#define PPP_COUNT_UTILS_H

#include "matrix.h"

int countStarsInRow(struct matrix_t *matrix, int rowIndex) {
    int counter = 0;
    for (int columnIndex = 0; columnIndex < matrix->columns; ++columnIndex) {
        if (matrix->stars[rowIndex][columnIndex]) {
            ++counter;
        }
    }

    return counter;
}

int countStarsInColumn(struct matrix_t *matrix, int columnIndex) {
    int counter = 0;
    for (int rowIndex = 0; rowIndex < matrix->rows; ++rowIndex) {
        if (matrix->stars[rowIndex][columnIndex]) {
            ++counter;
        }
    }

    return counter;
}

int *countColumnsStars(struct matrix_t *matrix) {
    int *columnsStarsAmount = (int *) calloc(matrix->columns, sizeof(int));
    if (columnsStarsAmount == NULL) {
        printf("Exit from countColumnsStars. Unable to allocate memory for columnsStarsAmount");
        return NULL;
    }

    int columnIndexLeft = 0;
    int columnIndexRight = matrix->columns - 1;

    while (columnIndexLeft < columnIndexRight) {
        columnsStarsAmount[columnIndexLeft] = countStarsInColumn(matrix, columnIndexLeft);
        columnsStarsAmount[columnIndexRight] = countStarsInColumn(matrix, columnIndexRight);

        ++columnIndexLeft;
        --columnIndexRight;
    }

    return columnsStarsAmount;
}

int countStars(struct matrix_t *matrix) {
    int starsAmount = 0;

    int rowIndexLeft = 0;
    int rowIndexRight = matrix->columns - 1;

    while (rowIndexLeft < rowIndexRight) {
        starsAmount += countStarsInRow(matrix, rowIndexLeft);
        starsAmount += countStarsInRow(matrix, rowIndexRight);

        ++rowIndexLeft;
        --rowIndexRight;
    }

    return starsAmount;
}

#endif //PPP_COUNT_UTILS_H
