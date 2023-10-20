#include "matrix.hpp"
#include <iostream>

int main() {
    Matrix mat1(4, 1);
    mat1.assign({
        {1},
        {2},
        {3},
        {4}});
    mat1.print();

    std::cout << mat1.data[1][0];
}