#include "ui/matrix_dialog.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
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

            // Always use "." as the decimal point, whatever the computer's
            // language is. Otherwise "2,5" could be typed but read back as 0.
            auto *validator = new QDoubleValidator(cell);
            validator->setLocale(QLocale::c());
            cell->setValidator(validator);

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
        // If a cell has a problem, stay open so the user can fix it.
        if (!readCells())
            return;

        accept();
    });
}

// Reads every cell into the matrix.
// Returns false (after showing a message) if a cell is not a valid number.
bool MatrixDialog::readCells()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            QLineEdit *cell = cellAt(i, j);
            const QString text = cell->text().trimmed();

            // An empty cell means 0.
            if (text.isEmpty())
            {
                matrix.set(i, j, 0.0);
                continue;
            }

            bool ok = false;
            const double value = text.toDouble(&ok);

            if (!ok)
            {
                QMessageBox::warning(
                    this, "Matrix Calculator",
                    QString("Row %1, column %2 is not a valid number.")
                        .arg(i + 1)
                        .arg(j + 1));
                cell->setFocus();
                cell->selectAll();
                return false;
            }

            matrix.set(i, j, value);
        }
    }

    return true;
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
