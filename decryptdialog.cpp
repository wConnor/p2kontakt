#include "decryptdialog.hpp"
#include "ui_decryptdialog.h"

DecryptDialog::DecryptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DecryptDialog)
{
    ui->setupUi(this);
}

DecryptDialog::~DecryptDialog()
{
    delete ui;
}

void DecryptDialog::on_button_decrypt_clicked()
{
    ui->input_plaintext->setText(QString::fromStdString(crypto.decrypt(ui->input_encrypted->toPlainText().toStdString())));
}

Identity *DecryptDialog::get_identity() const
{
    return identity;
}

void DecryptDialog::set_identity(Identity *new_identity)
{
    identity = new_identity;
    ui->label_user->setText("Decrypting as " + QString::fromStdString(identity->get_userid()));
}
