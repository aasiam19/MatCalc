#pragma once

#include "matrix/matrix.h"

// Math operations on matrices.
// These are plain functions: they take matrices in and give a new matrix back.
// They know nothing about the GUI.
// If something is wrong (for example, sizes do not match),
// they throw std::invalid_argument with a readable message.

// Returns a + b (the matrices must have the same size).
Matrix add(const Matrix &a, const Matrix &b);

// Returns a - b (the matrices must have the same size).
Matrix subtract(const Matrix &a, const Matrix &b);

// The operations the user can choose in the window.
// Returns every number of a multiplied by the scalar (a plain number).
Matrix multiplyByScalar(const Matrix &a, double scalar);


// Returns the transpose of a: rows become columns and columns become rows.
// A 2 x 3 matrix becomes 3 x 2.
Matrix transpose(const Matrix &a);

// Returns a x b (matrix multiplication).
// The columns of a must equal the rows of b.
// A (2 x 3) times a (3 x 4) gives a (2 x 4) result.
Matrix multiply(const Matrix &a, const Matrix &b);

// The operations the user can choose in the window.
enum class Operation
{
    Addition,
    Subtraction,
    ScalarMultiplication,
    Transpose,
    Multiplication
};

// Runs the chosen operation and returns the result.
// Operations that need only one matrix ignore b.
// Only the scalar operation uses the scalar number.
// Throws an exception with a readable message if it cannot be done.
Matrix calculate(Operation operation, const Matrix &a, const Matrix &b,
                 double scalar = 1.0);
