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

struct matrix_t subtractInRow(struct matrix_t *matrix, int minimum, int rowIndex) {
    int columnIndexLeft = 0;
    int columnIndexRight = (*matrix).rows - 1;
    while (columnIndexLeft < columnIndexRight) {
        (*matrix).matrix[rowIndex][columnIndexLeft] -= minimum;
        (*matrix).matrix[rowIndex][columnIndexRight] -= minimum;

        ++columnIndexLeft;
        --columnIndexRight;
    }
    return (*matrix);
}

struct matrix_t subtractInColumn(struct matrix_t *matrix, int minimum, int columnIndex) {
    int rowIndexLeft = 0;
    int rowIndexRight = (*matrix).rows - 1;

    while (rowIndexLeft < rowIndexRight) {
        (*matrix).matrix[rowIndexLeft][columnIndex] -= minimum;
        (*matrix).matrix[rowIndexRight][columnIndex] -= minimum;

        ++rowIndexLeft;
        --rowIndexRight;
    }
    return (*matrix);
}

void subtractColumnMinimum(struct matrix_t matrix, const int *columnMinimums) {
    int minimum;
    for (int columnIndex = 0; columnIndex < matrix.columns; ++columnIndex) {
        minimum = columnMinimums[columnIndex];

        matrix = subtractInColumn(&matrix, minimum, columnIndex);
    }
}

void subtractRowMinimum(struct matrix_t matrix, const int *rowMinimums) {
    int minimum;
    for (int rowIndex = 0; rowIndex < matrix.rows; ++rowIndex) {
        minimum = rowMinimums[rowIndex];

        matrix = subtractInRow(&matrix, minimum, rowIndex);
    }
}

#endif //PPP_MATH_UTILS_H
