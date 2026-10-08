#include "matrix/matrix.h"

#include <stdexcept>

Matrix::Matrix(int rows, int columns)
{
    if (rows <= 0 || columns <= 0)
        throw std::invalid_argument("Matrix size must be at least 1x1");

    data.resize(rows, std::vector<double>(columns, 0.0));
}

int Matrix::rows() const
{
    return static_cast<int>(data.size());
}

int Matrix::columns() const
{
    return static_cast<int>(data[0].size());
}

void Matrix::set(int row, int column, double value)
{
    checkIndex(row, column);
    data[row][column] = value;
}

double Matrix::get(int row, int column) const
{
    checkIndex(row, column);
    return data[row][column];
}

void Matrix::checkIndex(int row, int column) const
{
    if (row < 0 || row >= rows() || column < 0 || column >= columns())
        throw std::out_of_range("Matrix index out of range");
}
