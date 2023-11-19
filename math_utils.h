//
// Created by avmd on 11.11.23.
//

#ifndef PPP_MATH_UTILS_H
#define PPP_MATH_UTILS_H

int min(int first, int second) {
    return first < second
           ? first
           : second;
}

void addInRow(int **array, int value, int rowIndex, int columnsAmount) {
    int columnIndexLeft = 0;
    int columnIndexRight = columnsAmount - 1;


    while (columnIndexLeft < columnIndexRight) {
        array[rowIndex][columnIndexLeft] += value;
        array[rowIndex][columnIndexRight] += value;

        ++columnIndexLeft;
        --columnIndexRight;
    }

    if (columnsAmount % 2 == 1) {
        array[rowIndex][columnsAmount / 2] += value;
    }
}

void subtractInRow(struct matrix_t *matrix, int value, int rowIndex) {
    int columnIndexLeft = 0;
    int columnIndexRight = matrix->rows - 1;
    while (columnIndexLeft < columnIndexRight) {
        matrix->matrix[rowIndex][columnIndexLeft] -= value;
        matrix->matrix[rowIndex][columnIndexRight] -= value;

        ++columnIndexLeft;
        --columnIndexRight;
    }

    if (matrix->columns % 2 == 1) {
        matrix->matrix[rowIndex][matrix->columns / 2] += value;
    }
}

void subtractInColumn(int **array, int value, int rowsAmount, int columnIndex) {
    int rowIndexLeft = 0;
    int rowIndexRight = rowsAmount - 1;

    while (rowIndexLeft < rowIndexRight) {
        array[rowIndexLeft][columnIndex] -= value;
        array[rowIndexRight][columnIndex] -= value;

        ++rowIndexLeft;
        --rowIndexRight;
    }

    if (rowsAmount % 2 == 1) {
        array[rowsAmount / 2][columnIndex] += value;
    }
}

void subtractColumnMinimum(struct matrix_t *matrix, const int *columnMinimums) {
    int minimum;
    for (int columnIndex = 0; columnIndex < matrix->columns; ++columnIndex) {
        minimum = columnMinimums[columnIndex];

        subtractInColumn(matrix->matrix, minimum, matrix->rows, columnIndex);
    }
}

void subtractRowMinimum(struct matrix_t *matrix, const int *rowMinimums) {
    int minimum;
    for (int rowIndex = 0; rowIndex < matrix->rows; ++rowIndex) {
        minimum = rowMinimums[rowIndex];

        subtractInRow(matrix, minimum, rowIndex);
    }
}

#endif //PPP_MATH_UTILS_H
