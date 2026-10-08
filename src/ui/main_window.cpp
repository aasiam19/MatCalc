#include "ui/main_window.h"
#include "ui/matrix_dialog.h"
#include "ui/ui_helpers.h"

#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

// Settings in one place, so they are easy to change later.
namespace
{
    constexpr int kMinSize = 1;    // smallest allowed rows / columns
    constexpr int kMaxSize = 10;   // largest allowed rows / columns
}

// The constructor only sets the window up.
// The real work is done in the functions below.
MainWindow::MainWindow()
{
    setWindowTitle("Matrix Calculator");
    resize(800, 500);

    setupUi();
}

// Creates every widget and puts it into a layout.
void MainWindow::setupUi()
{
    // The main layout stacks everything from top to bottom.
    auto *mainLayout = new QVBoxLayout(this);

    // ---- Title ----
    mainLayout->addWidget(makeLabel("MATRIX CALCULATOR"));

    // ---- Matrix name row: [Matrix Name:] [ input ] ----
    auto *nameLayout = new QHBoxLayout;
    nameInput = makeInput();
    nameInput->setText("Mat A");
    nameLayout->addWidget(makeLabel("Matrix Name:"));
    nameLayout->addWidget(nameInput);
    mainLayout->addLayout(nameLayout);

    // ---- Size row: [Rows:] [ 2 ] [Columns:] [ 2 ] ----
    // These are plain text boxes where the user types a number.
    // The validator only lets the user type whole numbers in the allowed range.
    auto *sizeLayout = new QHBoxLayout;

    rowsInput = makeInput();
    rowsInput->setText("2");
    rowsInput->setFixedWidth(60);
    rowsInput->setAlignment(Qt::AlignCenter);
    rowsInput->setValidator(new QIntValidator(kMinSize, kMaxSize, rowsInput));

    columnsInput = makeInput();
    columnsInput->setText("2");
    columnsInput->setFixedWidth(60);
    columnsInput->setAlignment(Qt::AlignCenter);
    columnsInput->setValidator(new QIntValidator(kMinSize, kMaxSize, columnsInput));

    sizeLayout->addWidget(makeLabel("Rows:"));
    sizeLayout->addWidget(rowsInput);
    sizeLayout->addWidget(makeLabel("Columns:"));
    sizeLayout->addWidget(columnsInput);
    sizeLayout->addStretch();   // pushes everything to the left
    mainLayout->addLayout(sizeLayout);

    // ---- OPEN button: opens the matrix input dialog ----
    QPushButton *openButton = makeButton("OPEN");
    mainLayout->addWidget(openButton);

    // When the button is clicked, call onOpenClicked() on this window.
    connect(openButton, &QPushButton::clicked, this, &MainWindow::onOpenClicked);

    // ---- List of saved matrices ----
    mainLayout->addWidget(makeLabel("Stored matrices (double-click to edit):"));

    matrixList = new QListWidget;
    mainLayout->addWidget(matrixList);

    // Double-clicking a matrix in the list opens it for editing.
    connect(matrixList, &QListWidget::itemDoubleClicked,
            this, &MainWindow::onMatrixDoubleClicked);

    // ---- DELETE button: removes the selected matrix ----
    QPushButton *deleteButton = makeButton("DELETE");
    mainLayout->addWidget(deleteButton);

    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);

    // Fill the list for the first time (it is empty at start).
    updateMatrixList();
}

// Called when the user presses OPEN.
void MainWindow::onOpenClicked()
{
    // Read the name and remove spaces at the start and end.
    const QString name = nameInput->text().trimmed();

    // A matrix must have a name, so stop here if it is empty.
    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Matrix Calculator",
                             "Please enter a matrix name.");
        return;
    }

    // Check if a matrix with this name was already saved.
    // find() gives us a pointer to it, or nullptr if it does not exist.
    const Matrix *existing = store.find(name.toStdString());

    int rows = 0;
    int columns = 0;

    if (existing)
    {
        // An existing matrix keeps its saved size.
        // We also update the boxes so the screen shows the real size.
        rows = existing->rows();
        columns = existing->columns();
        rowsInput->setText(QString::number(rows));
        columnsInput->setText(QString::number(columns));
    }
    else
    {
        // A new matrix: read the size the user typed.
        // toInt() sets 'ok' to false if the text is not a number (for example empty).
        bool rowsOk = false;
        bool columnsOk = false;
        rows = rowsInput->text().toInt(&rowsOk);
        columns = columnsInput->text().toInt(&columnsOk);

        // Stop with a message if either size is missing or out of range.
        if (!rowsOk || !columnsOk
            || rows < kMinSize || rows > kMaxSize
            || columns < kMinSize || columns > kMaxSize)
        {
            QMessageBox::warning(this, "Matrix Calculator",
                                 QString("Rows and columns must be whole numbers from %1 to %2.")
                                     .arg(kMinSize)
                                     .arg(kMaxSize));
            return;
        }
    }

    // Create the input dialog.
    MatrixDialog dialog(name, rows, columns, this);

    // For an existing matrix, show its current values in the cells.
    if (existing)
        dialog.setValues(*existing);

    // Show the dialog and wait until the user closes it.
    // Only save if the user pressed DONE (Esc or X means cancel).
    if (dialog.exec() != QDialog::Accepted)
        return;

    // Save the matrix in the store under its name.
    // If the name already exists, the old matrix is replaced.
    store.put(name.toStdString(), dialog.result());

    updateMatrixList();
}

// Called when the user presses DELETE.
void MainWindow::onDeleteClicked()
{
    // Find which matrix is selected in the list.
    QListWidgetItem *item = matrixList->currentItem();

    // Nothing selected: tell the user and stop.
    if (!item)
    {
        QMessageBox::information(this, "Matrix Calculator",
                                 "Select a matrix in the list first.");
        return;
    }

    // The real matrix name is hidden inside the item (see updateMatrixList).
    const QString name = item->data(Qt::UserRole).toString();

    // Ask before deleting, because it cannot be undone.
    const auto answer = QMessageBox::question(
        this, "Delete matrix",
        QString("Delete matrix \"%1\"?").arg(name));

    if (answer != QMessageBox::Yes)
        return;

    // Remove it from the store and refresh the list.
    store.remove(name.toStdString());
    updateMatrixList();
}

// Called when the user double-clicks a matrix in the list.
void MainWindow::onMatrixDoubleClicked(QListWidgetItem *item)
{
    // Put the matrix name into the name box, then do the same as pressing OPEN.
    nameInput->setText(item->data(Qt::UserRole).toString());
    onOpenClicked();
}

// Rebuilds the list on screen from what is in the store.
void MainWindow::updateMatrixList()
{
    // Remove the old entries first.
    matrixList->clear();

    for (const std::string &n : store.names())
    {
        const Matrix *matrix = store.find(n);
        const QString name = QString::fromStdString(n);

        // The visible text shows the name and the size, like: Mat A  (2 x 2)
        auto *item = new QListWidgetItem(
            QString("%1  (%2 x %3)")
                .arg(name)
                .arg(matrix->rows())
                .arg(matrix->columns()));

        // The visible text has extra size info, so we also hide the plain name
        // inside the item. We read it back when editing or deleting.
        item->setData(Qt::UserRole, name);

        matrixList->addItem(item);
    }
}
