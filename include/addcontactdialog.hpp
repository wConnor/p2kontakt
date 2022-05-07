#ifndef ADDCONTACTDIALOG_HPP
#define ADDCONTACTDIALOG_HPP

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QMessageLogger>
#include "identity.hpp"

namespace Ui {
class AddContactDialog;
}

class AddContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddContactDialog(QWidget *parent = nullptr);
    ~AddContactDialog();

    Identity *get_identity() const;
    void set_identity(Identity *new_identity);

private slots:
    void on_button_browse_clicked();
    void on_buttons_confirm_accepted();
    void on_buttons_confirm_rejected();
private:
    Ui::AddContactDialog *ui;
    Identity *identity;
    QFile file;
};

#endif // ADDCONTACTDIALOG_HPP
