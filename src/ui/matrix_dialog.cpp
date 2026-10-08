#include "matrix_dialog.h"

#include <QGridLayout>
#include <QLineEdit>

MatrixDialog::MatrixDialog(const QString& matrixName, int rows, int columns)
{
    setWindowTitle(matrixName);
    resize(500, 400);

    QGridLayout *grid = new QGridLayout(this);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            QLineEdit *cell = new QLineEdit;
            cell->setFixedSize(70, 40);
            cell->setAlignment(Qt::AlignCenter);

            grid->addWidget(cell, i, j);
        }
    }
}
