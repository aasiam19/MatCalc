#include "ui/matrix_dialog.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

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
                focusNextCell(i, j);
            });
        }
    }

    auto *doneButton = new QPushButton(tr("Done"));
    doneButton->setAutoDefault(false);
    grid->addWidget(doneButton, rows, 0, 1, columns, Qt::AlignRight);

    connect(doneButton, &QPushButton::clicked, this, [this]()
    {
        readCells();
        accept();
    });
}

void MatrixDialog::readCells()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            bool ok = false;
            const double value = cellAt(i, j)->text().toDouble(&ok);
            if (ok)
                matrix.set(i, j, value);
        }
    }
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



// Puts the numbers of an existing matrix into the input cells.
void MatrixDialog::setValues(const Matrix &values)
{
    // Safety check: the sizes must match, otherwise do nothing.
    if (values.rows() != rows || values.columns() != columns)
        return;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            // Find the input box for this position.
            QLineEdit *cell = cellAt(i, j);
            if (!cell)
                continue;

            // Show the number as text.
            // 'g' with 15 digits keeps precision but avoids ugly trailing zeros.
            cell->setText(QString::number(values.get(i, j), 'g', 15));
        }
    }
}
