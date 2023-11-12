//
// Created by avmd on 11.11.23.
//

#ifndef PPP_MATRIX_H
#define PPP_MATRIX_H

struct matrix_t {
    int** startMatrix;
    int **matrix;
    bool **stars;
    int **selection;
    int **strokeMatrix;

    int columns;
    int rows;
};

struct cell_t {
    int rowIndex;
    int columnIndex;
};

#endif //PPP_MATRIX_H
