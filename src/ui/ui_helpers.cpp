#include "ui_helpers.h"

QPushButton* makeButton(const QString& text)
{
QPushButton *button = new QPushButton(text);
return button;
}

QLabel* makeLabel(const QString& text)
{
QLabel* label= new QLabel(text);
return label;
}
QLineEdit* makeInput()
{
QLineEdit *input = new QLineEdit;
return input;
}

