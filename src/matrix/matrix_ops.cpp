#include "matrix/matrix_ops.h"
#include <string>
#include <stdexcept>

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
   }

    // Only reached if a new Operation is added above but not handled here.
    throw std::logic_error("Unknown operation.");
}
