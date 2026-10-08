#include "matrix_dialog.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLineEdit>

MatrixDialog::MatrixDialog(const QString &matrixName, int rows, int columns,
                           QWidget *parent)
    : QDialog(parent)
    , rows(rows)
    , columns(columns)
    , matrix(rows, columns)
    , grid(new QGridLayout(this))
{
    setWindowTitle(matrixName);
    resize(500, 400);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            auto *cell = new QLineEdit;
            cell->setFixedSize(70, 40);
            cell->setAlignment(Qt::AlignCenter);
            cell->setValidator(new QDoubleValidator(cell));

            grid->addWidget(cell, i, j);

            connect(cell, &QLineEdit::returnPressed, this, [this, i, j]()
            {
                commitCell(i, j);
                focusNextCell(i, j);
            });

            // Also save when the user clicks away instead of pressing Enter
            connect(cell, &QLineEdit::editingFinished, this, [this, i, j]()
            {
                commitCell(i, j);
            });
        }
    }
}

void MatrixDialog::commitCell(int i, int j)
{
    auto *cell = cellAt(i, j);
    if (!cell)
        return;

    bool ok = false;
    const double value = cell->text().toDouble(&ok);
    if (ok)
        matrix.set(i, j, value);
}

void MatrixDialog::focusNextCell(int i, int j)
{
    const int next = i * columns + j + 1;
    if (next >= rows * columns)
        return;

    if (auto *cell = cellAt(next / columns, next % columns))
    {
        cell->setFocus();
        cell->selectAll();
    }
}

QLineEdit *MatrixDialog::cellAt(int row, int col) const
{
    QLayoutItem *item = grid->itemAtPosition(row, col);
    return item ? qobject_cast<QLineEdit *>(item->widget()) : nullptr;
}
