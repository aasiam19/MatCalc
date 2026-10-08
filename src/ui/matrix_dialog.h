#ifndef MATRIX_DIALOG_H
#define MATRIX_DIALOG_H

#include <QDialog>
#include "../matrix/matrix.h"

class MatrixDialog : public QDialog
{
private:
    Matrix matrix;

public:
    MatrixDialog(const QString& matrixName, int rows, int columns);
};

#endif
