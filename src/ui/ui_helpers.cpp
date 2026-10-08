#include "ui_helpers.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

QPushButton* makeButton(const QString &text, QWidget *parent)
{
    return new QPushButton(text, parent);
}

QLabel* makeLabel(const QString &text, QWidget *parent)
{
    return new QLabel(text, parent);
}

QLineEdit* makeInput(const QString &placeholder, QWidget *parent)
{
    auto *input = new QLineEdit(parent);
    input->setPlaceholderText(placeholder);
    return input;
}
