#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>


using namespace std;

struct cell_t {
    int rowIndex;
    int columnIndex;
};


class Solution {
    int result;
    vector<vector<int>> dataMatrix;
    vector<vector<bool>> solutionMatrix;
public:


    Solution(int result,
             vector<vector<int>> dataMatrix,
             vector<vector<bool>> solutionMatrix) {
        this->result = result;
        this->dataMatrix = std::move(dataMatrix);
        this->solutionMatrix = std::move(solutionMatrix);
    }

    void printSolutionInfo() {
        cout << "\n\t\t === INFO === \n";
        cout << "\tMinimal path length = " << result << endl;
        cout << "\tResult matrix:" << endl;

        for (int rowIndex = 0; rowIndex < dataMatrix.size(); ++rowIndex) {
            cout << "\t";
            for (int columnIndex = 0; columnIndex < dataMatrix[rowIndex].size(); ++columnIndex) {
                if (solutionMatrix[rowIndex][columnIndex]) {
                    cout << " [" << dataMatrix[rowIndex][columnIndex] << "]";
                } else {
                    bool isPreviousOptimal = columnIndex != 0 && solutionMatrix[rowIndex][columnIndex - 1];
                    if (isPreviousOptimal) {
                        cout << " " << dataMatrix[rowIndex][columnIndex];
                    } else {
                        cout << "  " << dataMatrix[rowIndex][columnIndex];
                    }
                }
            }

            cout << endl;
        }
    }
};

class Matrix {
    vector<vector<int>> startMatrix;
    vector<vector<int>> matrix;

    vector<vector<bool>> stars;

    vector<vector<int>> selection;
    vector<vector<int>> strokeMatrix;

    int columns;
    int rows;

public:
    explicit Matrix(vector<vector<int>> startMatrix) {
        this->startMatrix = startMatrix;
        matrix = std::move(startMatrix);

        rows = matrix.size();
        columns = matrix[0].size();

        stars = vector<vector<bool>>(rows, vector<bool>(columns));
    }

    Solution solve() {
        normalize();

        calculateStartSystemOfIndependentZeros();

        mainCalculation();

        return {calculateFinalResult(), startMatrix, stars};
    }

private:
    void normalize() {
        vector<int> columnMinimums = findColumnMinimums();
        subtractColumnMinimum(matrix, columnMinimums);

        vector<int> rowMinimums = findRowMinimums();
        subtractRowMinimum(matrix, rowMinimums);
    }

    void calculateStartSystemOfIndependentZeros() {
        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
                if (matrix[rowIndex][columnIndex] == 0) {
                    int count = countStarsInRow(rowIndex);
                    count += countStarsInColumn(columnIndex);

                    if (count == 0) {
                        stars[rowIndex][columnIndex] = true;
                    }
                }
            }
        }
    }

    void mainCalculation() {
        int starsAmount = calculateStars();

        while (starsAmount < columns) {
            makeIteration();

            starsAmount = calculateStars();
        }
    }

    int calculateFinalResult() {
        int result = 0;

        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
                if (stars[rowIndex][columnIndex]) {
                    result += startMatrix[rowIndex][columnIndex];
                }
            }
        }

        return result;
    }

    void makeIteration() {
        selection = vector<vector<int>>(rows, vector<int>(columns));
        strokeMatrix = vector<vector<int>>(rows, vector<int>(columns));

        vector<int> selectedColumns = getSelectedColumns();
        updateColumnSelection(selectedColumns);

        cell_t stroke = findNextZeroStroke(selectedColumns);

        buildLChain(stroke);
    }

    void buildLChain(struct cell_t stroke) {
        int rowIndex = stroke.rowIndex;
        int columnIndex = stroke.columnIndex;

        while (rowIndex > 0 && rowIndex < rows
               && columnIndex > 0 && columnIndex < columns) {
            strokeMatrix[rowIndex][columnIndex] = 0;
            stars[rowIndex][columnIndex] = true;

            int rowIterator = rowIndex + 1;
            while (rowIterator < rows && !stars[rowIterator][columnIndex]) {
                ++rowIterator;
            }

            if (rowIterator < rows) {
                int columnIterator = columnIndex + 1;
                while (columnIterator < columns
                       && !strokeMatrix[rowIterator][columnIterator]) {
                    ++columnIterator;
                }

                if (columnIterator < columns) {
                    stars[rowIndex][columnIndex] = false;
                }

                columnIndex = columnIterator;
            }

            rowIndex = rowIterator;
        }
    }


    cell_t findNextZeroStroke(vector<int> &selectedColumns) {
        vector<int> selectedRows(rows);
        cell_t stroke = {-1, -1};

        while (true) {
            stroke = findStroke();

            if (stroke.rowIndex == -1) {
                updateMatrix(selectedRows, selectedColumns);

                stroke = findStroke();
            }

            strokeMatrix[stroke.rowIndex][stroke.columnIndex] = 1;

            cell_t star = getNextStarFromRow(stroke.rowIndex);

            if (star.rowIndex == -1) {
                break;
            }

            unselectColumn(star.columnIndex);
            selectedColumns[star.columnIndex] = 0;

            selectRow(star.rowIndex);
            selectedRows[star.rowIndex] = 1;
        }

        return stroke;
    }

    void unselectColumn(int columnIndex) {
        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            selection[rowIndex][columnIndex] -= 1;
        }
    }

    void selectRow(int rowIndex) {
        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            selection[rowIndex][columnIndex] += 1;
        }
    }

    cell_t getNextStarFromRow(int rowIndex) {
        cell_t star = {-1, -1};

        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            if (stars[rowIndex][columnIndex] == 1) {
                star.rowIndex = rowIndex;
                star.columnIndex = columnIndex;
            }
        }

        return star;
    }

    void updateMatrix(vector<int> &selectedRows,
                      vector<int> &selectedColumns) {
        int minimum = findMinimumValueFromSelection();

        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            if (selectedColumns[columnIndex] == 1) {
                addInColumn(matrix, -minimum, columnIndex);
            }
        }

        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            if (selectedRows[rowIndex] == 1) {
                addInRow(matrix, minimum, rowIndex);
            }
        }
    }

    int findMinimumValueFromSelection() {
        int minimum = INT_MAX;

        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
                if (selection[rowIndex][columnIndex] == 0) {
                    minimum = min(minimum, matrix[rowIndex][columnIndex]);
                }
            }
        }

        return minimum;
    }

    struct cell_t findStroke() {
        struct cell_t stroke = {-1, -1};

        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
                if (selection[rowIndex][columnIndex] == 0 &&
                    matrix[rowIndex][columnIndex] == 0) {
                    stroke.columnIndex = columnIndex;
                    stroke.rowIndex = rowIndex;

                    return stroke;
                }
            }
        }

        return stroke;
    }

    void selectColumn(int columnIndex) {
        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            selection[rowIndex][columnIndex] = true;
        }
    }

    void updateColumnSelection(vector<int> selectedColumns) {
        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            if (selectedColumns[columnIndex] == 1) {
                selectColumn(columnIndex);
            }
        }
    }

    vector<int> getSelectedColumns() {
        vector<int> starsInColumn(columns);
        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            starsInColumn[columnIndex] = countStarsInColumn(columnIndex);
        }

        return starsInColumn;
    }

    int calculateStars() {
        int amount = 0;
        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            amount += countStarsInRow(rowIndex);
        }

        return amount;
    }

    int countStarsInRow(int rowIndex) {
        int amount = 0;
        for (auto star: stars[rowIndex]) {
            amount += star;
        }

        return amount;
    }

    int countStarsInColumn(int columnIndex) {
        int amount = 0;
        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            amount += stars[rowIndex][columnIndex];
        }

        return amount;
    }

    vector<int> findColumnMinimums() {
        vector<int> columnMinimums(columns);

        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            int minimum = INT_MAX;

            for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
                minimum = min(minimum, matrix[rowIndex][columnIndex]);
            }

            columnMinimums[columnIndex] = minimum;
        }

        return columnMinimums;
    }

    void subtractColumnMinimum(vector<vector<int>> &array, vector<int> &columnMinimums) const {
        int minimum;
        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            minimum = columnMinimums[columnIndex];

            addInColumn(array, -minimum, columnIndex);
        }
    }

    void addInColumn(vector<vector<int>> &array, int value, int columnIndex) const {
        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            array[rowIndex][columnIndex] += value;
        }
    }

    vector<int> findRowMinimums() {
        vector<int> rowMinimums(rows);

        for (int rowIndex = 0; rowIndex < columns; ++rowIndex) {
            rowMinimums[rowIndex] = *min_element(matrix[rowIndex].begin(), matrix[rowIndex].end());;
        }

        return rowMinimums;
    }

    void subtractRowMinimum(vector<vector<int>> &array, vector<int> &rowMinimums) const {
        int minimum;
        for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            minimum = rowMinimums[rowIndex];

            addInRow(array, -minimum, rowIndex);
        }
    }

    void addInRow(vector<vector<int>> &array, int value, int rowIndex) const {
        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            array[rowIndex][columnIndex] += value;
        }
    }
};

vector<vector<int>> inputData(const char *dataFile) {
    FILE *fileDescriptor = fopen(dataFile, "r");
    if (fileDescriptor == nullptr) {
        printf("Unable to open file %s", dataFile);
        return {};
    }

    int rows;
    int columns;
    fscanf(fileDescriptor, "%d", &rows);
    fscanf(fileDescriptor, "%d", &columns);

    vector<vector<int>> matrix = vector<vector<int>>(rows, vector<int>(columns));

    for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
            fscanf(fileDescriptor, "%d", &matrix[rowIndex][columnIndex]);
        }
    }

    return matrix;
}

int main() {
    vector<vector<int>> matrix = inputData("../input.txt");

    Matrix calculator = Matrix(matrix);

    Solution solution = calculator.solve();

    solution.printSolutionInfo();

    return 0;
}
