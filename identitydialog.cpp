#include "identitydialog.hpp"
#include "ui_identitydialog.h"

IdentityDialog::IdentityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IdentityDialog)
{
    ui->setupUi(this);
    QDate min_date = QDate::currentDate().addDays(1);
    ui->input_date->setMinimumDate(min_date);
    ui->input_date->setDate(min_date);

    if (first_time) {
        QMessageBox::information(this, "First Time Creation", "The system has detected that this is either your first time running p2kontakt or you have no identities stored.\nYou must create an identity before continuing.");
    }
}

IdentityDialog::~IdentityDialog()
{
    delete ui;
}

void IdentityDialog::set_first_time(bool first_time)
{
    this->first_time = first_time;
}

void IdentityDialog::on_buttons_confirm_accepted()
{
    std::unique_ptr<Identity> identity = std::make_unique<Identity>();

    QDateTime date_of_expiry(ui->input_date->date(), QTime(0, 0, 0, 0));

    if (identity->create_identity(ui->input_userid->text(), date_of_expiry, ui->checkbox_never->isChecked(), ui->input_ip->text().toStdString(), ui->checkbox_fetch->isChecked())) {
        QMessageBox::information(this, "Created identity", "p2kontakt has successfully generated an identity for user \'" + ui->input_userid->text() + "\'.");
        close();
    } else {
        QMessageBox::critical(this, "Failed to create identity", "p2kontakt failed to create an identity. The userid field must be populated.");
    }
}


void IdentityDialog::on_buttons_confirm_rejected()
{
//      first_time ?
//        QMessageBox::critical(this, "Identity Creation", "Unable to close dialog: you must create at least one identity.") :
        close();
}

void IdentityDialog::on_checkbox_never_stateChanged(int state)
{
    ui->input_date->setDisabled(state);
}


void IdentityDialog::on_checkbox_fetch_stateChanged(int state)
{
    ui->input_ip->setDisabled(state);
}

