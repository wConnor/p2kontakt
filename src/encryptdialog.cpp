#include "encryptdialog.hpp"
#include "ui_encryptdialog.h"

EncryptDialog::EncryptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EncryptDialog)
{
    ui->setupUi(this);
}

EncryptDialog::~EncryptDialog()
{
    delete ui;
}

void EncryptDialog::on_button_encrypt_clicked()
{
    gpgme_key_t *recipients = new gpgme_key_t[2];
    recipients[0] = identity->get_key();
    recipients[1] = nullptr;
    ui->input_encrypted->setText(QString::fromStdString(crypto.encrypt(ui->input_plaintext->toPlainText().toStdString(), &recipients)));
}

Identity *EncryptDialog::get_identity() const
{
    return identity;
}

void EncryptDialog::set_identity(Identity *new_identity)
{
    identity = new_identity;
    ui->label_user->setText("Encrypting as " + QString::fromStdString(identity->get_userid()));
}
