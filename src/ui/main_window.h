#pragma once

#include "matrix/matrix_store.h"

#include <QWidget>

// We only *mention* these Qt classes here (forward declarations).
// The full definitions are included in main_window.cpp.
class QComboBox;
class QLineEdit;
class QListWidget;
class QListWidgetItem;

// The main window of the application.
class MainWindow : public QWidget
{
    // Needed by Qt so that this class can use signals and slots.
    Q_OBJECT

public:
    MainWindow();

private:
    // Builds all the buttons, inputs and layouts.
    void setupUi();

    // Runs when the user presses the OPEN button.
    void onOpenClicked();

    // Runs when the user presses the DELETE button.
    void onDeleteClicked();

    // Runs when the user presses the VIEW button.
    void onViewClicked();
    // Runs when the user presses the CALCULATE button.
    void onCalculateClicked();

    // Refreshes the matrix drop-down boxes from what is in the store.
    void updateMatrixChoices();

    // Offers to save a calculation result as a new named matrix.
    void saveResult(const Matrix &result);

    // Returns the name of the matrix selected in the list.
    // If nothing is selected, shows a message and returns an empty string.
    QString selectedMatrixName();
    // Runs when the user double-clicks a matrix in the list.
    void onMatrixDoubleClicked(QListWidgetItem *item);

    // Refreshes the list on screen from what is in the store.
    void updateMatrixList();

    // Widgets we need to access later, so they are kept as members.
    QLineEdit *nameInput;
    QLineEdit *rowsInput;
    QLineEdit *columnsInput;
    QListWidget *matrixList;
    // The operation picker: which operation, and on which matrices.
    QComboBox *operationBox;
    QComboBox *firstMatrixBox;
    QComboBox *secondMatrixBox;
    // All matrices the user has created. Lives as long as the window.
    MatrixStore store;
};
