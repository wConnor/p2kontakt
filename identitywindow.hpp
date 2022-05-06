#ifndef IDENTITYWINDOW_HPP
#define IDENTITYWINDOW_HPP

#include <QWidget>
#include <QMessageBox>
#include <QDebug>

#include "identity.hpp"

#include <chrono>
#include <ctime>

namespace Ui {
class IdentityWindow;
}

class IdentityWindow : public QDialog
{
    Q_OBJECT

public:
    explicit IdentityWindow(QWidget *parent = nullptr);
    ~IdentityWindow();
    void set_first_time(bool first_time);
    void on_checkbox_never_clicked();
    void on_buttons_confirm_accepted();

private:
    Ui::IdentityWindow *ui;
    bool first_time;
};

#endif // IDENTITYWINDOW_HPP
