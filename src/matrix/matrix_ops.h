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
