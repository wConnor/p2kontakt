#include "authoriseipdialog.hpp"
#include "ui_authoriseipdialog.h"

AuthoriseIPDialog::AuthoriseIPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthoriseIPDialog)
{
    ui->setupUi(this);
}

AuthoriseIPDialog::~AuthoriseIPDialog()
{
    delete ui;
}

void AuthoriseIPDialog::on_checkbox_current_stateChanged(int state)
{
    ui->input_ip->setDisabled(state);

    if (state) {
        QMessageBox::information(this, "Note", "Authorising the current IP requires the use of the internet and will connect to an external party to retrieve it.");
    }
}

Identity *AuthoriseIPDialog::get_identity() const
{
    return identity;
}

void AuthoriseIPDialog::set_identity(Identity *new_identity)
{
    identity = new_identity;
}

void AuthoriseIPDialog::on_buttons_confirm_accepted()
{
    ui->checkbox_current->isChecked() ?
                identity->authorise_ip("", true) :
                identity->authorise_ip(ui->input_ip->text().toStdString(), false);
}

