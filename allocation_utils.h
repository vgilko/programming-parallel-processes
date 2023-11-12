//
// Created by avmd on 11.11.23.
//

#ifndef PPP_ALLOCATION_UTILS_H
#define PPP_ALLOCATION_UTILS_H

void **allocate(int elementSize, int rowsAmount, int columnsAmount) {
    void **result = calloc(elementSize, rowsAmount);

    if (result != NULL) {
        for (int rowIndex = 0; rowIndex < rowsAmount; ++rowIndex) {
            result[rowIndex] = calloc(elementSize, columnsAmount);
        }
    }

    return result;
}

#endif //PPP_ALLOCATION_UTILS_H
