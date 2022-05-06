#ifndef DECRYPTDIALOG_H
#define DECRYPTDIALOG_H

#include <QDialog>
#include "identity.hpp"

namespace Ui {
class DecryptDialog;
}

class DecryptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DecryptDialog(QWidget *parent = nullptr);
    ~DecryptDialog();

    Identity *get_identity() const;
    void set_identity(Identity *new_identity);

private slots:
    void on_button_decrypt_clicked();

private:
    Cryptography crypto;
    Ui::DecryptDialog *ui;
    Identity *identity;
};

#endif // DECRYPTDIALOG_H
