#pragma once

#include <QDialog>
#include "matrix.h"   // whatever your Matrix class header is called

class QGridLayout;
class QLineEdit;

class MatrixDialog : public QDialog
{
    Q_OBJECT

public:
    MatrixDialog(const QString &matrixName, int rows, int columns,
                 QWidget *parent = nullptr);

private:
    void commitCell(int i, int j);
    void focusNextCell(int i, int j);
    QLineEdit *cellAt(int row, int col) const;

    int rows;
    int columns;
    Matrix matrix;
    QGridLayout *grid;

public:
    const Matrix &result() const { return matrix; }   // so the caller can read the values

private:
    void commitAll();
};
