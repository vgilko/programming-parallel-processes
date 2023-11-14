//
// Created by avmd on 11.11.23.
//

#ifndef PPP_ALLOCATION_UTILS_H
#define PPP_ALLOCATION_UTILS_H

void **allocate(int elementSize, int rowsAmount, int columnsAmount) {
    void **result = calloc(rowsAmount, sizeof(int **));

    if (result != NULL) {
        for (int rowIndex = 0; rowIndex < rowsAmount; ++rowIndex) {
            result[rowIndex] = calloc(columnsAmount, elementSize);
            if (result[rowIndex] == NULL) {
                printf("allocate() error: Unable to allocate %d columns for %d row", columnsAmount, rowIndex);
                return NULL;
            }
        }
    } else {
        printf("allocate() error: Unable to allocate %d rows\n", rowsAmount);
    }

    return result;
}

void free2d(void **array, int rowsAmount) {
    for (int rowIndex = 0; rowIndex < rowsAmount; ++rowIndex) {
        if (array[rowIndex] != NULL) {
            free(array[rowIndex]);
        }
    }

    if (array != NULL) {
        free(array);
    }
}

#endif //PPP_ALLOCATION_UTILS_H
