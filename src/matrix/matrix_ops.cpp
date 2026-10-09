#include "matrix/matrix_ops.h"

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

Matrix calculate(Operation operation, const Matrix &a, const Matrix &b)
{
    // Nothing is connected yet, so every operation reports an error.
    // We connect them one at a time in Phase 3.
    (void)a;
    (void)b;

    switch (operation)
    {
    case Operation::Addition:
    case Operation::Subtraction:
        break;
    }

    throw std::logic_error("This operation is not connected yet.");
}
