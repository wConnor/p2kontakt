#ifndef ENCRYPTDIALOG_H
#define ENCRYPTDIALOG_H

#include <QDialog>
#include "identity.hpp"

namespace Ui {
class EncryptDialog;
}

class EncryptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncryptDialog(QWidget *parent = nullptr);
    ~EncryptDialog();

    Identity *get_identity() const;
    void set_identity(Identity *new_identity);

private slots:
    void on_button_encrypt_clicked();

private:
    Cryptography crypto;
    Ui::EncryptDialog *ui;
    Identity *identity;
};

#endif // ENCRYPTDIALOG_H
