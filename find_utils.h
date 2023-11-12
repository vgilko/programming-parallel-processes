//
// Created by avmd on 11.11.23.
//

#ifndef PPP_FIND_UTILS_H
#define PPP_FIND_UTILS_H

int *findColumnMinimums(struct matrix_t matrix) {
    int *columnMinimums = (int *) calloc(sizeof(int), matrix.columns);

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
    int *rowMinimums = (int *) calloc(sizeof(int), matrix.rows);

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

bool isZeroCell(struct matrix_t *matrix, int columnIndex, int rowIndex) {
    return (*matrix).matrix[rowIndex][columnIndex] == 0;
}

int canBeStarMarked(struct matrix_t *matrix, int columnIndex, int rowIndex) {
    int starsAmount = countStarsInRow((*matrix), rowIndex) +
                      countStarsInColumn((*matrix), columnIndex);

    return starsAmount == 0;
}

bool isShouldBeStarMarked(int columnIndex, int rowIndex, struct matrix_t *matrix) {
    return isZeroCell(matrix, columnIndex, rowIndex) &&
           canBeStarMarked(matrix, columnIndex, rowIndex);
}


int findStars(struct matrix_t matrix) {
    int activeStarsAmount = 0;

    for (int columnIndex = 0; columnIndex < matrix.columns; ++columnIndex) {
        for (int rowIndex = 0; rowIndex < matrix.rows; ++rowIndex) {
            if (isShouldBeStarMarked(columnIndex, rowIndex, &matrix)) {
                matrix.stars[rowIndex][columnIndex] = true;
                ++activeStarsAmount;
            }
        }
    }

    return activeStarsAmount;
}


struct cell_t findStroke(struct matrix_t matrix) {
    struct cell_t stroke;

    for (int rowIndex = 0; rowIndex < matrix.rows; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < matrix.columns; ++columnIndex) {
            if (matrix.selection[rowIndex][columnIndex] == 0 &&
                matrix.matrix[rowIndex][columnIndex] == 0) {
                stroke.columnIndex = columnIndex;
                stroke.rowIndex = rowIndex;

                return stroke;
            }
        }
    }

    return stroke;
}

int findLocalMinimum(struct matrix_t *matrix) {
    int localMinimum = INT_MAX;

    for (int columnIndex = 0; columnIndex < (*matrix).columns; ++columnIndex) {
        for (int rowIndex = 0; rowIndex < (*matrix).rows; ++rowIndex) {
            if ((*matrix).selection[rowIndex][columnIndex] == 0) {
                localMinimum = min((*matrix).matrix[rowIndex][columnIndex], localMinimum);
            }
        }
    }

    return localMinimum;
}


#endif //PPP_FIND_UTILS_H
