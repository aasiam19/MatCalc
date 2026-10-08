#pragma once

#include <QString>

class QPushButton;
class QLabel;
class QLineEdit;
class QWidget;

[[nodiscard]] QPushButton* makeButton(const QString &text, QWidget *parent = nullptr);
[[nodiscard]] QLabel*      makeLabel(const QString &text, QWidget *parent = nullptr);
[[nodiscard]] QLineEdit*   makeInput(const QString &placeholder = QString(),
                                     QWidget *parent = nullptr);
