#pragma once
#include <iostream>
#include <vector>

class Matrix {
    public:
        std::vector<std::vector<float>> data;

        Matrix(int rows, int cols, float init = 0.0f) : data(rows, std::vector<float>(cols, init)) {}

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

        void identity() {
            for (int row = 0; row < data.size(); row++) {
                for (int col = 0; col < data[0].size(); col++) {
                    if (row == col) {data[row][col] = 1.0f;}
                    else            {data[row][col] = 0.0f;}
                }
            }
        }

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