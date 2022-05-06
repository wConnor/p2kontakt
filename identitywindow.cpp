#include "identitywindow.hpp"
#include "ui_identitywindow.h"

IdentityWindow::IdentityWindow(QWidget *parent) : QDialog(parent), ui(new Ui::IdentityWindow)
{
    ui->setupUi(this);
    QDate min_date = QDate::currentDate().addDays(1);
    ui->input_date->setMinimumDate(min_date);
    ui->input_date->setDate(min_date);

    if (first_time) {
        QMessageBox::information(this, "First Time Creation", "The system has detected that this is either your first time running p2kontakt or you have no identities stored.\nYou must create an identity before continuing.");
    }
}

IdentityWindow::~IdentityWindow()
{
    delete ui;
}

void IdentityWindow::set_first_time(bool first_time)
{
    this->first_time = first_time;
}

void IdentityWindow::on_checkbox_never_clicked()
{
    ui->input_date->setDisabled(true);
}

void IdentityWindow::on_buttons_confirm_accepted()
{
    qInfo() << "Test.";
    std::unique_ptr<Identity> identity = std::make_unique<Identity>();



    identity->create_identity(ui->input_name->text(), ui->input_email->text(), ui->input_date->date());



}
