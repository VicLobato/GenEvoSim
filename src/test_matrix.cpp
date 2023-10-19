#include "matrix.hpp"
#include <iostream>

int main() {
    Matrix mat1(3, 2);
    mat1.assign({{2,4},{6,8},{10,12}});
    mat1.print();

    Matrix mat2(2, 3);
    mat2.assign({{1,2,3},{3,2,1}});
    mat2.print();

    Matrix matmul = mat1 * mat2;
    std::cout << "Multiplied = \n";
    matmul.print();
}