#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

QPushButton* makeButton(const QString& text);
QLabel* makeLabel(const QString& text);
QLineEdit* makeInput();

#endif
