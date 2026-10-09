#pragma once

#include "matrix/matrix.h"

#include <QDialog>

// A small window that shows a matrix as a read-only table.
// Used to view stored matrices now, and to show results later.
class MatrixViewDialog : public QDialog
{
public:
    MatrixViewDialog(const QString &title, const Matrix &matrix,
                     QWidget *parent = nullptr);
};
