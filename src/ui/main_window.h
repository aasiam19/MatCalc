#pragma once

#include "matrix/matrix_store.h"

#include <QWidget>

class MainWindow : public QWidget
{
public:
    MainWindow();

private:
    MatrixStore store;
};
