#include "ui/main_window.h"
#include "ui/matrix_dialog.h"
#include "ui/ui_helpers.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>

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

    QLabel *storedLabel = makeLabel("Stored matrices: (none)");
    mainLayout->addWidget(storedLabel);

    QObject::connect(okButton, &QPushButton::clicked, this, [=]()
    {
        const QString name = nameInput->text().trimmed();
        if (name.isEmpty())
        {
            QMessageBox::warning(this, "Matrix Calculator",
                                 "Please enter a matrix name.");
            return;
        }

        MatrixDialog dialog(
            name,
            rowsBox->value(),
            columnsBox->value(),
            this
        );

        if (dialog.exec() != QDialog::Accepted)
            return;

        store.put(name.toStdString(), dialog.result());

        QStringList names;
        for (const std::string &n : store.names())
            names << QString::fromStdString(n);
        storedLabel->setText("Stored matrices: " + names.join(", "));
    });
}
