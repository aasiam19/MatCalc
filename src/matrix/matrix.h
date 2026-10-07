#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix
{
private:
std::vector<std::vector<double>> data;

public:
Matrix(int rows, int columns);
int rows() const;
int columns() const;

void set(int row, int column, double value);
double get(int row, int column) const;
};
#endif
