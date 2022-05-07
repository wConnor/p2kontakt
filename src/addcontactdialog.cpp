#include "addcontactdialog.hpp"
#include "ui_addcontactdialog.h"

AddContactDialog::AddContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddContactDialog)
{
    ui->setupUi(this);
}

AddContactDialog::~AddContactDialog()
{
    delete ui;
}

void AddContactDialog::on_buttons_confirm_rejected()
{
    close();
}

Identity *AddContactDialog::get_identity() const
{
    return identity;
}

void AddContactDialog::set_identity(Identity *new_identity)
{
    identity = new_identity;
}

void AddContactDialog::on_button_browse_clicked()
{
    const std::filesystem::path public_key_dir = PUBLIC_KEY_HOME;
    QString file_name = QFileDialog::getOpenFileName(this, "Select Contact File", QString::fromStdString(public_key_dir.string()));
    QFile contact_file(file_name);

    if (!contact_file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Warning", "Unable to open contact file: " + contact_file.errorString());
    } else {
        ui->input_location->setText(contact_file.fileName());
        file.setFileName(file_name);
        contact_file.close();
    }
}

void AddContactDialog::on_buttons_confirm_accepted()
{
    identity->add_contact(file) ?
                QMessageBox::information(this, "Added", "Successfully added contact.") :
                QMessageBox::critical(this, "Error", "Failed to add contact due to an unknown reason.");
}

