#pragma once
#include <iostream>
#include <vector>

// Custom matrix class to make my life easier when projecting points
class Matrix {
    public:
        std::vector<std::vector<float>> data;

        Matrix(int rows = 3, int cols = 3, float init = 0.0f) : data(rows, std::vector<float>(cols, init)) {}

        // Let a matrix multiply another
        // No division as you can multiply by inverse
        Matrix operator*(const Matrix &other) {
            int _rows = data.size();
            int _cols = other.data[0].size();
            int iter = other.data.size();
            Matrix result(_rows, _cols);
            
            for (int i = 0; i < _rows; i++) {
                for (int j = 0; j < _cols; j++) {
                    for (int k = 0; k < iter; k++) {
                        result.data[i][j] += data[i][k] * other.data[k][j];
                    }
                }
            }

            return result;
        }

        // Let a matrix add to another
        // No negative operator as we can negate and add to achieve subtraction
        Matrix operator+(const Matrix &other) {
            int _rows = data.size();
            int _cols = data[0].size();
            Matrix result(_rows, _cols);

            for (int i = 0; i < _rows; i++) {
                for (int j = 0; j < _cols; j++) {
                    result.data[i][j] += data[i][j] + other.data[i][j];
                }
            }

            return result;
        }

        // Just yield an identity matrix
        void identity() {
            for (int row = 0; row < data.size(); row++) {
                for (int col = 0; col < data[0].size(); col++) {
                    if (row == col) {data[row][col] = 1.0f;}
                    else            {data[row][col] = 0.0f;}
                }
            }
        }

        // Print in an easy to read manner, used for debugging
        void print() const {
            for (const auto &row : data) {
                for (const auto &elem : row) {
                    std::cout << elem << " ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
};