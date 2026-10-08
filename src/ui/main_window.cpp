#include "main_window.h"
#include "ui_helpers.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QComboBox>

MainWindow::MainWindow()
{
    setWindowTitle("Matrix Calculator");
    resize(800, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = makeLabel("MATRIX CALCULATOR");
    mainLayout->addWidget(title);

    QHBoxLayout *matrixLayout = new QHBoxLayout;

    QLabel *matrixLabel = makeLabel("Matrix:");
    QComboBox *matrixBox = new QComboBox;

    matrixBox->addItem("Mat A");
    matrixBox->addItem("Mat B");
    matrixBox->addItem("Mat C");
    matrixBox->addItem("Mat D");

    matrixLayout->addWidget(matrixLabel);
    matrixLayout->addWidget(matrixBox);

    mainLayout->addLayout(matrixLayout);

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

    QPushButton *okButton = makeButton("OK");
    mainLayout->addWidget(okButton);
}
