#include "matrix/matrix_ops.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


namespace
{
    // Stops with an error if two matrices do not have the same size.
    // Used by both add() and subtract().
    void requireSameSize(const Matrix &a, const Matrix &b)
    {
        if (a.rows() != b.rows() || a.columns() != b.columns())
            throw std::invalid_argument("Matrices must have the same size.");
    }
}

Matrix add(const Matrix &a, const Matrix &b)
{
    requireSameSize(a, b);

    // Start with an empty matrix of the same size.
    Matrix result(a.rows(), a.columns());

    // Add the numbers one position at a time.
    for (int i = 0; i < a.rows(); i++)
        for (int j = 0; j < a.columns(); j++)
            result.set(i, j, a.get(i, j) + b.get(i, j));

    return result;
}


Matrix subtract(const Matrix &a, const Matrix &b)
{
    requireSameSize(a, b);

    Matrix result(a.rows(), a.columns());

    for (int i = 0; i < a.rows(); i++)
        for (int j = 0; j < a.columns(); j++)
            result.set(i, j, a.get(i, j) - b.get(i, j));

    return result;
}



Matrix multiplyByScalar(const Matrix &a, double scalar)
{
    Matrix result(a.rows(), a.columns());

    // Multiply the numbers one position at a time.
    for (int i = 0; i < a.rows(); i++)
        for (int j = 0; j < a.columns(); j++)
            result.set(i, j, a.get(i, j) * scalar);

    return result;
}



Matrix transpose(const Matrix &a)
{
    // The result has the rows and columns swapped.
    Matrix result(a.columns(), a.rows());

    // The number at row i, column j moves to row j, column i.
    for (int i = 0; i < a.rows(); i++)
        for (int j = 0; j < a.columns(); j++)
            result.set(j, i, a.get(i, j));

    return result;
}



Matrix multiply(const Matrix &a, const Matrix &b)
{
    // The rule: columns of a must equal rows of b.
    if (a.columns() != b.rows())
    {
        throw std::invalid_argument(
            "Cannot multiply a " + std::to_string(a.rows()) + " x "
            + std::to_string(a.columns()) + " matrix by a "
            + std::to_string(b.rows()) + " x " + std::to_string(b.columns())
            + " matrix.\nThe columns of the first matrix ("
            + std::to_string(a.columns())
            + ") must equal the rows of the second matrix ("
            + std::to_string(b.rows()) + ").");
    }

    // The result has a's rows and b's columns.
    Matrix result(a.rows(), b.columns());

    for (int i = 0; i < a.rows(); i++)
    {
        for (int j = 0; j < b.columns(); j++)
        {
            // Entry (i, j) = row i of a times column j of b:
            // multiply the pairs and add them up.
            double sum = 0.0;
            for (int k = 0; k < a.columns(); k++)
                sum += a.get(i, k) * b.get(k, j);

            result.set(i, j, sum);
        }
    }

    return result;
}

double determinant(const Matrix &a)
{
    // Only square matrices have a determinant.
    if (a.rows() != a.columns())
    {
        throw std::invalid_argument(
            "The determinant needs a square matrix (same number of rows and "
            "columns).\nThis matrix is " + std::to_string(a.rows()) + " x "
            + std::to_string(a.columns()) + ".");
    }

    const int n = a.rows();

    // Work on a plain copy, so the original matrix is never changed.
    std::vector<std::vector<double>> m(n, std::vector<double>(n));
    double biggest = 0.0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            m[i][j] = a.get(i, j);
            biggest = std::max(biggest, std::fabs(m[i][j]));
        }
    }

    // A matrix of only zeros has determinant 0.
    if (biggest == 0.0)
        return 0.0;

    // A number this small (compared to the biggest number in the matrix)
    // is treated as zero. This hides tiny rounding errors, so a singular
    // matrix gives exactly 0 instead of something like 6.6e-16.
    const double tolerance = biggest * 1e-12;

    // Gaussian elimination: turn the matrix into a triangle.
    // The determinant is then the product of the numbers on the diagonal.
    double det = 1.0;

    for (int col = 0; col < n; col++)
    {
        // Pick the row (at or below this one) with the biggest number in
        // this column. This keeps the calculation accurate.
        int pivot = col;
        for (int r = col + 1; r < n; r++)
            if (std::fabs(m[r][col]) > std::fabs(m[pivot][col]))
                pivot = r;

        // No usable number in this column: the determinant is 0.
        if (std::fabs(m[pivot][col]) <= tolerance)
            return 0.0;

        // Swapping two rows flips the sign of the determinant.
        if (pivot != col)
        {
            std::swap(m[pivot], m[col]);
            det = -det;
        }

        det *= m[col][col];

        // Subtract a multiple of this row from the rows below,
        // so that the column becomes zero below the diagonal.
        for (int r = col + 1; r < n; r++)
        {
            const double factor = m[r][col] / m[col][col];
            for (int c = col; c < n; c++)
                m[r][c] -= factor * m[col][c];
        }
    }

    return det;
}


Matrix calculate(Operation operation, const Matrix &a, const Matrix &b,
                 double scalar)
{
    // Pick the right math function for the chosen operation.
    switch (operation)
    {
    case Operation::Addition:
        return add(a, b);
    case Operation::Subtraction:
        return subtract(a, b);
    case Operation::ScalarMultiplication:
        return multiplyByScalar(a, scalar);
    case Operation::Transpose:
        return transpose(a);
    case Operation::Multiplication:
        return multiply(a, b);
    case Operation::Determinant:
        // The determinant is a single number, not a matrix.
        // The window calls determinant() directly for this operation.
        throw std::logic_error(
            "The determinant is a number, not a matrix. Use determinant().");


   }

    // Only reached if a new Operation is added above but not handled here.
    throw std::logic_error("Unknown operation.");
}
