#include <QInputDialog>
#include "matrix/matrix_ops.h"
#include <exception>
#include <QComboBox>
#include "ui/main_window.h"
#include "ui/matrix_dialog.h"
#include "ui/matrix_view_dialog.h"
#include "ui/ui_helpers.h"
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDoubleValidator>

// Settings in one place, so they are easy to change later.
namespace
{
    constexpr int kMinSize = 1;    // smallest allowed rows / columns
    constexpr int kMaxSize = 10;   // largest allowed rows / columns

    // True if the operation needs a second matrix (Addition, Subtraction).
    bool usesSecondMatrix(Operation operation)
    {
        return operation == Operation::Addition
            || operation == Operation::Subtraction
            || operation == Operation::Multiplication;

 }

    // True if the operation needs a plain number (Scalar multiplication).
    bool usesNumber(Operation operation)
    {
        return operation == Operation::ScalarMultiplication;
    }

    // True if the operation gives a single number instead of a matrix.
    bool producesNumber(Operation operation)
    {
        return operation == Operation::Determinant;
    }


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
    // ---- VIEW button: shows the selected matrix in a table ----
    QPushButton *viewButton = makeButton("VIEW");
    mainLayout->addWidget(viewButton);

    connect(viewButton, &QPushButton::clicked, this, &MainWindow::onViewClicked);
    // ---- Operation picker ----
    mainLayout->addWidget(makeLabel("Operation:"));

    // Row 1: which operation to do.
    operationBox = new QComboBox;

    // The second value is the hidden data: which Operation this entry means.
    operationBox->addItem("Addition", static_cast<int>(Operation::Addition));
    operationBox->addItem("Subtraction", static_cast<int>(Operation::Subtraction));    mainLayout->addWidget(operationBox);
    operationBox->addItem("Scalar multiplication", static_cast<int>(Operation::ScalarMultiplication));
    operationBox->addItem("Transpose", static_cast<int>(Operation::Transpose));
    operationBox->addItem("Multiplication", static_cast<int>(Operation::Multiplication));
    operationBox->addItem("Determinant", static_cast<int>(Operation::Determinant));

	// Row 2: [First matrix:] [ box ] [Second matrix:] [ box ] [Number:] [ box ]
    // The second matrix and the number never show together:
    // onOperationChanged() decides which one is visible.
    auto *pickLayout = new QHBoxLayout;
    firstMatrixBox = new QComboBox;
    secondMatrixBox = new QComboBox;
    secondMatrixLabel = makeLabel("Second matrix:");

    scalarLabel = makeLabel("Number:");
    scalarInput = makeInput();
    scalarInput->setText("2");
    scalarInput->setFixedWidth(80);
    scalarInput->setAlignment(Qt::AlignCenter);
    scalarInput->setValidator(new QDoubleValidator(scalarInput));

    pickLayout->addWidget(makeLabel("First matrix:"));
    pickLayout->addWidget(firstMatrixBox);
    pickLayout->addWidget(secondMatrixLabel);
    pickLayout->addWidget(secondMatrixBox);
    pickLayout->addWidget(scalarLabel);
    pickLayout->addWidget(scalarInput);
    mainLayout->addLayout(pickLayout);

    // Whenever the chosen operation changes, show or hide the right boxes.
    connect(operationBox, &QComboBox::currentIndexChanged,
            this, &MainWindow::onOperationChanged);
    onOperationChanged();   // set the correct boxes for the first operation
    // CALCULATE button: runs the chosen operation.
    QPushButton *calculateButton = makeButton("CALCULATE");
    mainLayout->addWidget(calculateButton);

    connect(calculateButton, &QPushButton::clicked,
            this, &MainWindow::onCalculateClicked);
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
    // Which matrix is selected? (empty if none)
    const QString name = selectedMatrixName();
    if (name.isEmpty())
        return;

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
	// Keep the operation picker's drop-down boxes in sync with the list.
    	updateMatrixChoices();

}
// Called when the user presses VIEW.
void MainWindow::onViewClicked()
{
    const QString name = selectedMatrixName();
    if (name.isEmpty())
        return;

    // Look the matrix up in the store.
    const Matrix *matrix = store.find(name.toStdString());
    if (!matrix)
        return;

    // Show it in a read-only table window.
    MatrixViewDialog dialog(name, *matrix, this);
    dialog.exec();
}

// Returns the name of the selected matrix in the list.
// If nothing is selected, tells the user and returns an empty string.
QString MainWindow::selectedMatrixName()
{
    QListWidgetItem *item = matrixList->currentItem();

    if (!item)
    {
        QMessageBox::information(this, "Matrix Calculator",
                                 "Select a matrix in the list first.");
        return QString();
    }

    // The plain name is hidden inside the item (see updateMatrixList).
    return item->data(Qt::UserRole).toString();
}

// Called when the user presses CALCULATE.
void MainWindow::onCalculateClicked()
{
    // No matrices saved yet, so there is nothing to calculate with.
    if (firstMatrixBox->count() == 0)
    {
        QMessageBox::information(this, "Matrix Calculator",
                                 "Create at least one matrix first.");
        return;
    }

    // Read which operation the user picked (stored as hidden data).
    const auto operation =
        static_cast<Operation>(operationBox->currentData().toInt());

    // Look up the first matrix. A pointer is nullptr if it was not found.
    const QString firstName = firstMatrixBox->currentData().toString();
    const Matrix *first = store.find(firstName.toStdString());

    if (!first)
    {
        QMessageBox::warning(this, "Matrix Calculator",
                             "The chosen matrix no longer exists.");
        return;
    }

    // These depend on the operation.
    // One-matrix operations ignore 'second', so it defaults to the first matrix.
    const Matrix *second = first;
    double scalar = 1.0;
    QString extraText;   // the second matrix or number, for the window title

    if (usesNumber(operation))
    {
        // toDouble() sets 'ok' to false if the text is not a number.
        bool ok = false;
        scalar = scalarInput->text().toDouble(&ok);

        if (!ok)
        {
            QMessageBox::warning(this, "Matrix Calculator",
                                 "Please enter a number in the Number box.");
            return;
        }

        extraText = scalarInput->text().trimmed();
    }
    else if (usesSecondMatrix(operation))
    {
        const QString secondName = secondMatrixBox->currentData().toString();
        second = store.find(secondName.toStdString());

        if (!second)
        {
            QMessageBox::warning(this, "Matrix Calculator",
                                 "The chosen matrix no longer exists.");
            return;
        }

        extraText = secondName;
    }

    // The math can fail (for example, sizes that do not match).
    // 'try' runs the math. If it throws an error, 'catch' receives it
    // and we show its message instead of letting the program crash.
    try
    {
        // Operations that give a single number are shown in a message box
        // instead of the result table (a number cannot be saved as a matrix).
        if (producesNumber(operation))
        {
            const double value = determinant(*first);

            // 'g' with 10 digits shows -2 as "-2" and hides rounding noise.
            QMessageBox::information(
                this, "Determinant",
                QString("Determinant of %1 = %2")
                    .arg(firstName)
                    .arg(value, 0, 'g', 10));
            return;
        }

        const Matrix result = calculate(operation, *first, *second, scalar);
        // Build the result window title, like: Addition: Mat A, B
        QString title = operationBox->currentText() + ": " + firstName;
        if (!extraText.isEmpty())
            title += ", " + extraText;

        MatrixViewDialog dialog(title, result, this);
        dialog.exec();

        // After the user closes the result window, offer to save it.
        saveResult(result);
    }
    catch (const std::exception &error)
    {
        QMessageBox::warning(this, "Matrix Calculator",
                             QString::fromUtf8(error.what()));
    }
}


// Refills the two matrix drop-down boxes from the store.
void MainWindow::updateMatrixChoices()
{
    // Remember what the user had selected, so we can select it again.
    const QString firstOld = firstMatrixBox->currentData().toString();
    const QString secondOld = secondMatrixBox->currentData().toString();

    firstMatrixBox->clear();
    secondMatrixBox->clear();

    // Add every stored matrix to both boxes.
    // The first QString is the shown text, the second is the hidden data.
    for (const std::string &n : store.names())
    {
        const QString name = QString::fromStdString(n);
        firstMatrixBox->addItem(name, name);
        secondMatrixBox->addItem(name, name);
    }

    // Restore the old selection if that matrix still exists.
    // findData() returns -1 when it is not found.
    const int firstIndex = firstMatrixBox->findData(firstOld);
    if (firstIndex >= 0)
        firstMatrixBox->setCurrentIndex(firstIndex);

    const int secondIndex = secondMatrixBox->findData(secondOld);
    if (secondIndex >= 0)
        secondMatrixBox->setCurrentIndex(secondIndex);
}
// Offers to save a result as a new named matrix.
void MainWindow::saveResult(const Matrix &result)
{
    const auto answer = QMessageBox::question(
        this, "Save result", "Save this result as a new matrix?");

    if (answer != QMessageBox::Yes)
        return;

    // Keep asking for a name until we get a usable one, or the user cancels.
    while (true)
    {
        // 'ok' becomes false if the user presses Cancel.
        bool ok = false;
        const QString name = QInputDialog::getText(
            this, "Save result", "Name for the new matrix:",
            QLineEdit::Normal, "Result", &ok).trimmed();

        if (!ok)
            return;

        // An empty name is not allowed: ask again.
        if (name.isEmpty())
        {
            QMessageBox::warning(this, "Matrix Calculator",
                                 "Please enter a matrix name.");
            continue;
        }

        // The name is already used: ask before replacing that matrix.
        if (store.contains(name.toStdString()))
        {
            const auto replace = QMessageBox::question(
                this, "Matrix already exists",
                QString("\"%1\" already exists. Replace it?").arg(name));

            // No: go back and ask for a different name.
            if (replace != QMessageBox::Yes)
                continue;
        }

        // Save the result, refresh the list and the drop-down boxes, and finish.
        store.put(name.toStdString(), result);
        updateMatrixList();
        return;
    }
}

// Shows or hides the boxes that the chosen operation needs.
void MainWindow::onOperationChanged()
{
    const auto operation =
        static_cast<Operation>(operationBox->currentData().toInt());

    // The second matrix and the number are only shown when the operation uses them.
    secondMatrixLabel->setVisible(usesSecondMatrix(operation));
    secondMatrixBox->setVisible(usesSecondMatrix(operation));
    scalarLabel->setVisible(usesNumber(operation));
    scalarInput->setVisible(usesNumber(operation));
}

