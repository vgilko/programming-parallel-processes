//
// Created by avmd on 14.11.23.
//

#ifndef PPP_IO_UTILS_H
#define PPP_IO_UTILS_H

struct matrix_t *inputData(const char *dataFile) {
    FILE *fileDescriptor = fopen(dataFile, "r");
    if (fileDescriptor == NULL) {
        printf("Unable to open file %s", dataFile);
        return NULL;
    }

    struct matrix_t *matrix = (struct matrix_t *) calloc(1, sizeof(struct matrix_t));


    fscanf(fileDescriptor, "%d", &matrix->rows);
    fscanf(fileDescriptor, "%d", &matrix->columns);

    matrix->matrix = (int **) allocate(sizeof(int), matrix->rows, matrix->columns);
    if (matrix->matrix == NULL) {
        return matrix;
    }

    for (int rowIndex = 0; rowIndex < matrix->rows; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < matrix->columns; ++columnIndex) {
            fscanf(fileDescriptor, "%d", &matrix->matrix[rowIndex][columnIndex]);
        }
    }

    return matrix;
}

void print(struct matrix_t *matrix) {
    for (int rowIndex = 0; rowIndex < matrix->rows; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < matrix->columns; ++columnIndex) {
            printf("%d ", matrix->matrix[rowIndex][columnIndex]);
        }

        printf("\n");
    }
}

#endif //PPP_IO_UTILS_H
