#ifndef AUTHORISEIPDIALOG_HPP
#define AUTHORISEIPDIALOG_HPP

#include <QDialog>
#include <QMessageBox>

#include "identity.hpp"

namespace Ui {
class AuthoriseIPDialog;
}

class AuthoriseIPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthoriseIPDialog(QWidget *parent = nullptr);
    ~AuthoriseIPDialog();

    Identity *get_identity() const;
    void set_identity(Identity *new_identity);

private slots:
    void on_checkbox_current_stateChanged(int state);

    void on_buttons_confirm_accepted();

private:
    Ui::AuthoriseIPDialog *ui;
    Identity *identity;
};

#endif // AUTHORISEIPDIALOG_HPP
