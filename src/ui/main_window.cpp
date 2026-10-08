#include "main_window.h"
#include "matrix_dialog.h"
#include "ui_helpers.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLineEdit>

MainWindow::MainWindow()
{
    setWindowTitle("Matrix Calculator");
    resize(800, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = makeLabel("MATRIX CALCULATOR");
    mainLayout->addWidget(title);

    QHBoxLayout *nameLayout = new QHBoxLayout;

    QLabel *nameLabel = makeLabel("Matrix Name:");
    QLineEdit *nameInput = makeInput();
    nameInput->setText("Mat A");

    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameInput);

    mainLayout->addLayout(nameLayout);

    QHBoxLayout *sizeLayout = new QHBoxLayout;

    QLabel *rowsLabel = makeLabel("Rows:");
    QSpinBox *rowsBox = new QSpinBox;
    rowsBox->setRange(1, 10);
    rowsBox->setValue(2);

    QLabel *columnsLabel = makeLabel("Columns:");
    QSpinBox *columnsBox = new QSpinBox;
    columnsBox->setRange(1, 10);
    columnsBox->setValue(2);

    sizeLayout->addWidget(rowsLabel);
    sizeLayout->addWidget(rowsBox);
    sizeLayout->addWidget(columnsLabel);
    sizeLayout->addWidget(columnsBox);

    mainLayout->addLayout(sizeLayout);

    QPushButton *okButton = makeButton("OPEN");
    mainLayout->addWidget(okButton);

    QObject::connect(okButton, &QPushButton::clicked, this, [=]()
    {
        MatrixDialog dialog(
            nameInput->text(),
            rowsBox->value(),
            columnsBox->value()
        );

        dialog.exec();
    });
}
