#include "ui/matrix_view_dialog.h"
#include "ui/ui_helpers.h"

#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

MatrixViewDialog::MatrixViewDialog(const QString &title, const Matrix &matrix,
                                   QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(title);
    resize(500, 350);

    auto *layout = new QVBoxLayout(this);

    // A table with the same number of rows and columns as the matrix.
    auto *table = new QTableWidget(matrix.rows(), matrix.columns());

    // The user can look at the numbers but not change them.
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Make the cells stretch to fill the window.
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Copy every number from the matrix into the table.
    for (int i = 0; i < matrix.rows(); i++)
    {
        for (int j = 0; j < matrix.columns(); j++)
        {
            // 'g' with 10 digits shows 2.5 as "2.5" and 4 as "4",
            // and hides tiny rounding noise like 0.30000000000000004.
            auto *item = new QTableWidgetItem(
                QString::number(matrix.get(i, j), 'g', 10));
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
    }

    layout->addWidget(table);

    // CLOSE button: closes the window.
    QPushButton *closeButton = makeButton("CLOSE");
    layout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}
