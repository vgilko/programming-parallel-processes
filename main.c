#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <memory.h>

struct matrix_t {
    int **matrix;
    bool **stars;
    int columns;
    int rows;
};

int min(int first, int second) {
    return first < second
           ? first
           : second;
}

int *findColumnMinimums(struct matrix_t matrix) {
    int *columnMinimums = (int *) malloc(matrix.columns);

    if (columnMinimums == NULL) {
        printf("Unable to allocate memory for columnMinimums");
        return NULL;
    }

    for (int columnIndex = 0; columnIndex < matrix.columns; ++columnIndex) {
        int rowIndexLeft = 0;
        int rowIndexRight = matrix.rows - 1;

        int minimum = INT_MAX;
        while (rowIndexLeft < rowIndexRight) {
            int left = matrix.matrix[rowIndexLeft][columnIndex];
            int right = matrix.matrix[rowIndexRight][columnIndex];

            minimum = min(minimum, min(left, right));

            ++rowIndexLeft;
            --rowIndexRight;
        }

        columnMinimums[columnIndex] = minimum;
    }

    return columnMinimums;
}

int *findRowMinimums(struct matrix_t matrix) {
    int *rowMinimums = (int *) malloc(matrix.rows);

    if (rowMinimums == NULL) {
        printf("Unable to allocate memory for rowMinimums");
        return NULL;
    }

    for (int rowIndex = 0; rowIndex < matrix.columns; ++rowIndex) {
        int columnIndexLeft = 0;
        int columnIndexRight = matrix.rows - 1;

        int minimum = INT_MAX;
        while (columnIndexLeft < columnIndexRight) {
            int left = matrix.matrix[rowIndex][columnIndexLeft];
            int right = matrix.matrix[rowIndex][columnIndexRight];

            minimum = min(minimum, min(left, right));

            ++columnIndexLeft;
            --columnIndexRight;
        }

        rowMinimums[rowIndex] = minimum;
    }

    return rowMinimums;
}

void subtractColumnMinimum(struct matrix_t matrix, const int *columnMinimums) {
    int minimum;
    for (int columnIndex = 0; columnIndex < matrix.columns; ++columnIndex) {
        int rowIndexLeft = 0;
        int rowIndexRight = matrix.rows - 1;

        minimum = columnMinimums[columnIndex];

        while (rowIndexLeft < rowIndexRight) {
            matrix.matrix[rowIndexLeft][columnIndex] -= minimum;
            matrix.matrix[rowIndexRight][columnIndex] -= minimum;

            ++rowIndexLeft;
            --rowIndexRight;
        }
    }
}

void subtractRowMinimum(struct matrix_t matrix, const int *rowMinimums) {
    int minimum;
    for (int rowIndex = 0; rowIndex < matrix.rows; ++rowIndex) {
        int columnIndexLeft = 0;
        int columnIndexRight = matrix.rows - 1;

        minimum = rowMinimums[rowIndex];

        while (columnIndexLeft < columnIndexRight) {
            matrix.matrix[rowIndex][columnIndexLeft] -= minimum;
            matrix.matrix[rowIndex][columnIndexRight] -= minimum;

            ++columnIndexLeft;
            --columnIndexRight;
        }
    }
}

int countStarsInRow(struct matrix_t matrix, int rowIndex) {
    int counter = 0;
    for (int columnIndex = 0; columnIndex < matrix.columns; ++columnIndex) {
        if (matrix.stars[rowIndex][columnIndex]) {
            ++counter;
        }
    }

    return counter;
}

int countStarsInColumn(struct matrix_t matrix, int columnIndex) {
    int counter = 0;
    for (int rowIndex = 0; rowIndex < matrix.rows; ++rowIndex) {
        if (matrix.stars[rowIndex][columnIndex]) {
            ++counter;
        }
    }

    return counter;
}

bool allocateMemoryForStars(struct matrix_t matrix) {
    matrix.stars = (bool **) malloc(matrix.rows);

    if (matrix.stars == NULL) {
        printf("Unable to allocate memory for stars");
        return false;
    }
    for (int rowIndex = 0; rowIndex < matrix.rows; ++rowIndex) {
        matrix.stars[rowIndex] = (bool *) malloc(matrix.columns);
        if (matrix.stars[rowIndex] == NULL) {
            printf("Unable to allocate memory for stars row");
            return false;
        }

        memset(matrix.stars[rowIndex], 0, matrix.columns);
    }

    return true;
}

bool **findStars(struct matrix_t matrix) {
    for (int columnIndex = 0; columnIndex < matrix.columns; ++columnIndex) {
        for (int rowIndex = 0; rowIndex < matrix.rows; ++rowIndex) {
            if (matrix.matrix[rowIndex][columnIndex] == 0) {
                int starsAmount = countStarsInRow(matrix, rowIndex) +
                                  countStarsInColumn(matrix, columnIndex);

                if (starsAmount == 0) {
                    matrix.stars[rowIndex][columnIndex] = true;
                }
            }
        }
    }
}

int countStars(struct matrix_t matrix) {
    int counter = 0;

    for (int rowIndex = 0; rowIndex < matrix.rows; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < matrix.columns; ++columnIndex) {
            if (matrix.stars[rowIndex][columnIndex]) {
                ++counter;
            }
        }
    }

    return counter;
}


int findMinimum(struct matrix_t matrix) {
    bool isAllocatedForStars = allocateMemoryForStars(matrix);
    if (!isAllocatedForStars) {
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

    findStars(matrix);

    int starsAmount = countStars(matrix);
}


int main() {

    return 0;
}
