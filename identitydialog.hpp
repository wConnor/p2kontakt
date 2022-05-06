#ifndef IDENTITYDIALOG_HPP
#define IDENTITYDIALOG_HPP

#include "identity.hpp"

#include <QDebug>
#include <QMessageBox>

#include <chrono>
#include <ctime>

namespace Ui {
class IdentityDialog;
}

class IdentityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IdentityDialog(QWidget *parent = nullptr);
    ~IdentityDialog();
    void set_first_time(bool first_time);

private slots:
    void on_buttons_confirm_rejected();
    void on_buttons_confirm_accepted();
    void on_checkbox_never_stateChanged(int state);
    void on_checkbox_fetch_stateChanged(int state);

private:
    Ui::IdentityDialog *ui;
    bool first_time = false;
};

#endif // IDENTITYDIALOG_HPP
