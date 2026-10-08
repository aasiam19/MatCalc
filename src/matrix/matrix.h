#pragma once

#include <vector>

class Matrix
{
public:
    Matrix(int rows, int columns);

    int rows() const;
    int columns() const;

    void set(int row, int column, double value);
    double get(int row, int column) const;

private:
    void checkIndex(int row, int column) const;

    std::vector<std::vector<double>> data;
};
