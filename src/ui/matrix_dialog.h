#pragma once

#include "matrix/matrix.h"

#include <QDialog>

class QGridLayout;
class QLineEdit;

class MatrixDialog : public QDialog
{
    Q_OBJECT

public:
    MatrixDialog(const QString &matrixName, int rows, int columns,
                 QWidget *parent = nullptr);

        const Matrix &result() const { return matrix; }

    // Fills the input cells with the values of an existing matrix,
    // so the user can edit it instead of typing everything again.
    void setValues(const Matrix &values);
private:
    void readCells();
    void focusNextCell(int i, int j);
    QLineEdit *cellAt(int row, int col) const;

    int rows;
    int columns;
    Matrix matrix;
    QGridLayout *grid;
};
