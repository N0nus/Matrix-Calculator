#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <string>

class Matrix {
public:
    Matrix() : name(""), rows(0), columns(0), array(0, std::vector<float>(0)) {}
    Matrix(const std::string& name, size_t rws, size_t clmns);

    // Accessor and Mutator
    void setElement(size_t row, size_t col, float value);
    float getElement(size_t row, size_t col) const;
    void createMatrix();
    void multiplyRow(float multiplier, size_t row);
    float parseFraction(const std::string& fractionStr) const;
    void addRows(float multiplier, size_t row1, size_t row2);
    void swapRows(size_t row1, size_t row2);
    void attemptSolution();
    bool isEmpty() const;
    Matrix transpose() const;
    Matrix add(const Matrix& other) const;
    Matrix multiply(const Matrix& other) const;
    Matrix duplicate(const std::string& newName) const;

    // Print matrix
    void print() const;
    
    // Get the name of the matrix
    std::string getName() const { return name; }

private:
    std::string name;
    size_t rows;
    size_t columns;
    std::vector<std::vector<float>> array; // 2D vector of floats

    // Helper function to parse fractions
};

Matrix::Matrix(const std::string& name, size_t rws, size_t clmns) 
    : name(name), rows(rws), columns(clmns), array(rws, std::vector<float>(clmns)) {
}

void Matrix::setElement(size_t row, size_t col, float value) {
    if (row < rows && col < columns) {
        array[row][col] = value;
    }
}

float Matrix::getElement(size_t row, size_t col) const {
    if (row < rows && col < columns) {
        return array[row][col];
    }
    return -1.0f; // Error value (using -1.0f to indicate an error)
}

void Matrix::print() const {
    std::cout << "Matrix " << name << ":\n";
    for (const auto& row : array) {
        for (const auto& elem : row) {
            std::cout << elem << "\t";
        }
        std::cout << std::endl;
    }
}

void Matrix::createMatrix() {
    std::cout << "Enter elements for matrix " << name << " (" << rows << "x" << columns << "):\n";
    for (size_t row = 0; row < rows; ++row) {
        bool validInput = false;
        while (!validInput) {
            std::cout << "Enter " << columns << " elements for row " << row + 1 << " (separated by spaces): ";
            std::string line;
            std::getline(std::cin, line); // Read the entire line of input

            std::istringstream iss(line);
            std::vector<float> tempRow;
            float value;

            // Read all values from the input line
            while (iss >> value) {
                tempRow.push_back(value);
            }

            // Check if the number of elements is exactly what is needed
            if (tempRow.size() == columns) {
                // If the size is correct, copy the values to the matrix
                for (size_t col = 0; col < columns; ++col) {
                    array[row][col] = tempRow[col];
                }
                validInput = true; // Input was valid, exit the loop
            } else if (tempRow.size() < columns) {
                // Not enough elements, prompt the user to retry
                std::cerr << "Error: Not enough elements provided for row " << row + 1 << ". Please try again.\n";
            } else {
                // Too many elements, prompt the user to retry
                std::cerr << "Error: Too many elements provided for row " << row + 1 << ". Please try again.\n";
            }
        }
    }
}

void Matrix::multiplyRow(float multiplier, size_t row) {
    if (row < rows) { // Check if the row index is valid
        for (size_t col = 0; col < columns; ++col) {
            array[row][col] *= multiplier;
        }
        std::cout << "Multiplied row " << row + 1 << " by " << multiplier << ".\n";
        print();
    } else {
        std::cerr << "Error: Row index " << row + 1 << " is out of bounds.\n";
    }
}

void Matrix::addRows(float multiplier, size_t row1, size_t row2) {
    if (row1 < rows && row2 < rows) { // Check if the row index is valid
        for (size_t col = 0; col < columns; ++col) {
            array[row1][col] += array[row2][col] * multiplier;
        }
        if (multiplier != 1) {
            std::cout << "Multiplied row " << row2 + 1 << " by " << multiplier << " and added it to row " << row1 + 1 << ".\n";
            print();
        } else {
            std::cout << "Added row " << row2 + 1 << " to row " << row1 + 1 << ".\n";
            print();
        }
    } else {
        std::cerr << "Error: Row index " << row1 + 1 << " or " << row2 + 1 << " is out of bounds.\n";
    }
}

float Matrix::parseFraction(const std::string& fractionStr) const {
    size_t pos = fractionStr.find('/');
    if (pos != std::string::npos) {
        std::string numeratorStr = fractionStr.substr(0, pos);
        std::string denominatorStr = fractionStr.substr(pos + 1);
        try {
            float numerator = std::stof(numeratorStr);
            float denominator = std::stof(denominatorStr);
            if (denominator != 0) {
                return numerator / denominator;
            } else {
                throw std::invalid_argument("Denominator cannot be zero.");
            }
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid fraction format.\n";
            return 0.0f; // Default to 0 if parsing fails
        }
    } else {
        // Not a fraction, just convert to float
        try {
            return std::stof(fractionStr);
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid number format.\n";
            return 0.0f; // Default to 0 if parsing fails
        }
    }
}

void Matrix::swapRows(size_t row1, size_t row2) {
    if (row1 < rows && row2 < rows) { // Check if row indices are valid
        float tmp;
        for (size_t col = 0; col < columns; ++col) {
            tmp = array[row1][col];
            array[row1][col] = array[row2][col];
            array[row2][col] = tmp;
        }
        std::cout << "Swapped row " << row1 + 1 << " with row " << row2 + 1 << ".\n";
    } else {
        std::cerr << "Error: Row index " << row1 + 1 << " or " << row2 + 1 << " is out of bounds.\n";
    }
}

void Matrix::attemptSolution() {
    size_t lead = 0; // Index of the leading column

    // First, move zero rows to the bottom
    size_t nonZeroRow = 0;
    for (size_t r = 0; r < rows; ++r) {
        bool allZero = true;
        for (size_t c = 0; c < columns - 1; ++c) {
            if (array[r][c] != 0) {
                allZero = false;
                break;
            }
        }
        if (allZero) {
            // Swap this row with the row at 'nonZeroRow' which is the first zero row to be moved
            if (r != nonZeroRow) {
                swapRows(r, nonZeroRow);
            }
            ++nonZeroRow; // Move to the next row to be swapped with zero rows
        }
    }

    // Now, proceed with Gaussian elimination to RREF
    lead = 0; // Reset lead index to start processing columns

    for (size_t r = 0; r < rows; ++r) {
        // Find the pivot column
        while (lead < columns - 1 && array[r][lead] == 0) {
            ++lead;
            if (lead >= columns - 1) {
                return; // No more columns to process
            }
        }

        // If there is no non-zero element in this row, skip it
        if (lead >= columns - 1 || array[r][lead] == 0) {
            continue;
        }

        // Normalize the pivot row
        float pivot = array[r][lead];
        multiplyRow(1.0f / pivot, r);

        // Eliminate the column above and below the pivot
        for (size_t i = 0; i < rows; ++i) {
            if (i != r) {
                float factor = array[i][lead];
                addRows(-factor, i, r);
            }
        }

        ++lead; // Move to the next column
    }
}

Matrix Matrix::transpose() const {
    Matrix transposedMatrix(name, columns, rows);

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < columns; ++c) {
            transposedMatrix.setElement(c, r, array[r][c]);
        }
    }

    return transposedMatrix;
}

Matrix Matrix::add(const Matrix& other) const {
    if (rows != other.rows || columns != other.columns) {
        std::cerr << "Error: Matrices must have the same dimensions to be added.\n";
        return Matrix(); // Return an empty matrix
    }

    Matrix result(name, rows, columns);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            result.setElement(i, j, array[i][j] + other.getElement(i, j));
        }
    }


    return result;
}

bool Matrix::isEmpty() const {
    return rows == 0 || columns == 0;
}

Matrix Matrix::multiply(const Matrix& other) const {
    if (columns != other.rows) {
        std::cerr << "Error: Number of columns in the first matrix must be equal to the number of rows in the second matrix.\n";
        return Matrix(); // Return an empty matrix
    }

    Matrix result(name, rows, other.columns);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < other.columns; ++j) {
            float sum = 0.0f;
            for (size_t k = 0; k < columns; ++k) {
                sum += array[i][k] * other.getElement(k, j);
            }
            result.setElement(i, j, sum);
        }
    }

    return result;
}

Matrix Matrix::duplicate(const std::string& newName) const {
    Matrix duplicated(newName, rows, columns);
    duplicated.array = array;
    return duplicated;
}

int main() {
    std::map<std::string, Matrix> matrices;
    bool running = true;

    while (running) {
        std::cout << "\nMatrix Operations Menu:\n";
        std::cout << "1. Initialize a new matrix\n";
        std::cout << "2. Create a matrix\n";
        std::cout << "3. Print a matrix\n";
        std::cout << "4. Multiply a row\n";
        std::cout << "5. Add rows\n";
        std::cout << "6. Swap rows\n";
        std::cout << "7. Solve\n";
        std::cout << "8. Transpose a matrix\n";
        std::cout << "9. Add two matrices\n";
        std::cout << "10. Multiply two matrices\n";
        std::cout << "11. Duplicate a matrix\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // To consume the newline character after entering choice
        
        switch (choice) {
            case 1: {
                std::string name;
                size_t rows, cols;
                std::cout << "Enter matrix name: ";
                std::getline(std::cin, name);
                std::cout << "Enter number of rows: ";
                std::cin >> rows;
                std::cout << "Enter number of columns: ";
                std::cin >> cols;
                std::cin.ignore(); // Consume newline
                matrices[name] = Matrix(name, rows, cols);
                break;
            }
            case 2: {
                std::string name;
                std::cout << "Enter matrix name to Create:\n";
                for (const auto& pair : matrices) {
                            std::cout << pair.second.getName() << std::endl;
                        }

                std::getline(std::cin, name);
                if (matrices.find(name) != matrices.end()) {
                    matrices[name].createMatrix();
                } else {
                    std::cerr << "Error: Matrix with name " << name << " does not exist.\n";
                }
                break;
            }
            case 3: {

                for (const auto& pair : matrices) {
                            std::cout << pair.second.getName() << std::endl;
                        }

                std::string name;
                std::cout << "Enter matrix name to print (or type 'ALL' to print all matrices): ";
                std::getline(std::cin, name);

                if (name == "ALL") {
                    // Print all matrices
                    if (matrices.empty()) {
                        std::cout << "No matrices to display.\n";
                    } else {
                        for (const auto& pair : matrices) {
                            pair.second.print();
                            std::cout << std::endl;
                        }
                    }
                } else {
                    // Print a specific matrix
                    if (matrices.find(name) != matrices.end()) {
                        matrices[name].print();
                    } else {
                        std::cerr << "Error: Matrix with name " << name << " does not exist.\n";
                    }
                }
                break;
            }
            case 4: {
                std::string name;
                std::string multiplierStr;
                size_t row;
                std::cout << "Enter matrix name:\n";
                for (const auto& pair : matrices) {
                            std::cout << pair.second.getName() << std::endl;
                        }

                std::getline(std::cin, name);
                if (matrices.find(name) != matrices.end()) {
                    std::cout << "Enter multiplier (can be a fraction like 1/4): ";
                    std::getline(std::cin, multiplierStr);
                    float multiplier = matrices[name].parseFraction(multiplierStr);
                    std::cout << "Enter row index (1-based): ";
                    std::cin >> row;
                    std::cin.ignore(); // Consume newline
                    matrices[name].multiplyRow(multiplier, row - 1); // Convert 1-based to 0-based
                } else {
                    std::cerr << "Error: Matrix with name " << name << " does not exist.\n";
                }
                break;
            }
            case 5: {
                std::string name;
                std::string multiplierStr;
                size_t row1, row2;
                std::cout << "Enter matrix name:\n";
                for (const auto& pair : matrices) {
                            std::cout << pair.second.getName() << std::endl;
                        }

                std::getline(std::cin, name);
                if (matrices.find(name) != matrices.end()) {
                    std::cout << "Enter multiplier (can be a fraction like 1/4): ";
                    std::getline(std::cin, multiplierStr);
                    float multiplier = matrices[name].parseFraction(multiplierStr);
                    std::cout << "Enter row1 index, row to be added to (1-based): ";
                    std::cin >> row1;
                    std::cout << "Enter row2 index, row to add (1-based): ";
                    std::cin >> row2;
                    std::cin.ignore(); // Consume newline
                    matrices[name].addRows(multiplier, row1 - 1, row2 - 1); // Convert 1-based to 0-based
                    std::cout << "New Matrix: \n";
                    matrices[name].print();
                } else {
                    std::cerr << "Error: Matrix with name " << name << " does not exist.\n";
                }
                break;
            }
            case 6: {
                std::string name;
                size_t row1, row2;
                std::cout << "Enter matrix name:\n";
                for (const auto& pair : matrices) {
                            std::cout << pair.second.getName() << std::endl;
                        }
                std::getline(std::cin, name);
                if (matrices.find(name) != matrices.end()) {
                    std::cout << "Enter row1 to swap: ";
                    std::cin >> row1;
                    std::cout << "Enter row2 to swap: ";
                    std::cin >> row2;
                    matrices[name].swapRows(row1-1, row2-1);
                    std::cout << "New Matrix: \n";
                    matrices[name].print();
                }
                else {
                    std::cerr << "Error: Matrix with name " << name << " does not exist.\n";
                }
                break;

            }
            case 7: {
                std::string name;
                std::cout << "Enter matrix name: ";
                for (const auto& pair : matrices) {
                            std::cout << pair.second.getName() << std::endl;
                        }
                std::getline(std::cin, name);
                matrices[name].attemptSolution();
                break;
            }
            case 8: {
                std::string name;
                std::cout << "Enter matrix name:\n";
                for (const auto& pair : matrices) {
                    std::cout << pair.second.getName() << std::endl;
                }

                std::getline(std::cin, name);
                if (matrices.find(name) != matrices.end()) {
                    matrices[name] = matrices[name].transpose();
                    std::cout << "Transposed Matrix:\n";
                    matrices[name].print();
                } else {
                    std::cerr << "Error: Matrix with name " << name << " does not exist.\n";
                }
                break;
            }
            case 9: { // Add a new case for matrix addition
                std::string name1, name2;
                std::cout << "Enter the name of the first matrix (destination matrix):\n";
                for (const auto& pair : matrices) {
                    std::cout << pair.second.getName() << std::endl;
                }
                std::getline(std::cin, name1);

                std::cout << "Enter the name of the second matrix:\n";
                for (const auto& pair : matrices) {
                    std::cout << pair.second.getName() << std::endl;
                }
                std::getline(std::cin, name2);

                if (matrices.find(name1) != matrices.end() && matrices.find(name2) != matrices.end()) {
                    Matrix sum = matrices[name1].add(matrices[name2]);
                    if(sum.isEmpty()) {
                        break;
                    }
                    matrices[name1] = matrices[name1].add(matrices[name2]);
                    std::cout << "Matrix Sum:\n";
                    matrices[name1].print();
                } else {
                    std::cerr << "Error: One or both matrices do not exist.\n";
                }
                break;
            }
            case 10: { // Add a new case for matrix addition
                std::string name1, name2;
                std::cout << "Enter the name of the first matrix (destination matrix):\n";
                for (const auto& pair : matrices) {
                    std::cout << pair.second.getName() << std::endl;
                }
                std::getline(std::cin, name1);

                std::cout << "Enter the name of the second matrix:\n";
                for (const auto& pair : matrices) {
                    std::cout << pair.second.getName() << std::endl;
                }
                std::getline(std::cin, name2);

                if (matrices.find(name1) != matrices.end() && matrices.find(name2) != matrices.end()) {
                    Matrix product = matrices[name1].multiply(matrices[name2]);
                    if(product.isEmpty()) {
                        break;
                    }
                    matrices[name1] = matrices[name1].multiply(matrices[name2]);
                    std::cout << "Matrix Product:\n";
                    matrices[name1].print();
                } else {
                    std::cerr << "Error: One or both matrices do not exist.\n";
                }
                break;
            }
            case 11: {
                std::string name1, name2;
                std::cout << "Enter the name of the matrix to duplicate:\n";
                for (const auto& pair : matrices) {
                    std::cout << pair.second.getName() << std::endl;
                }
                std::getline(std::cin, name1);

                std::cout << "Enter the name of the new matrix:\n";
                std::getline(std::cin, name2);

                matrices[name2] = matrices[name1].duplicate(name2);
                break;
            }
            case 0: {
                running = false;
                break;
            }
            default:
                std::cerr << "Error: Invalid choice. Please try again.\n";
                
        }
    }
    return 0;
}