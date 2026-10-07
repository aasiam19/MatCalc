#include "matrix.h"

Matrix::Matrix(int rows, int columns)
{
data.resize(rows, std::vector<double>(columns, 0.0));
}
int Matrix::rows() const
{
return data.size();
}
int Matrix::columns() const
{
if (data.empty())
return 0;
return data[0].size();
}
void Matrix::set(int row, int column, double value)
{
data[row][column]=value;
}
double Matrix::get(int row, int column) const
{
return data[row][column];
}
