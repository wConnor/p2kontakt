#ifndef EXPORTDIALOG_HPP
#define EXPORTDIALOG_HPP

#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

#include "identity.hpp"

#ifdef _WIN32
    #define IDENTITY_HOME std::filesystem::current_path + std::filesytem::path::preferred_separator + "identities" + std::filesystem::path::preferred_separator;
    #define PUBLIC_KEY_HOME std::filesystem::current_path + std::filesytem::path::preferred_separator + "public_keys" + std::filesystem::path::preferred_separator;
#elif defined __unix__
    #define IDENTITY_HOME std::string(std::getenv("HOME")) + std::filesystem::path::preferred_separator + ".config" + std::filesystem::path::preferred_separator + "p2kontakt" + std::filesystem::path::preferred_separator + "identities" + std::filesystem::path::preferred_separator;
    #define PUBLIC_KEY_HOME std::string(std::getenv("HOME")) + std::filesystem::path::preferred_separator + ".config" + std::filesystem::path::preferred_separator + "p2kontakt" + std::filesystem::path::preferred_separator + "public_keys" + std::filesystem::path::preferred_separator;
#elif defined __APPLE__
    #define IDENTITY_HOME std::string(std::getenv("HOME")) + std::filesystem::path::preferred_separator + ".config" + std::filesystem::path::preferred_separator + "p2kontakt" + std::filesystem::path::preferred_separator + "identities" + std::filesystem::path::preferred_separator;
    #define PUBLIC_KEY_HOME std::string(std::getenv("HOME")) + std::filesystem::path::preferred_separator + ".config" + std::filesystem::path::preferred_separator + "p2kontakt" + std::filesystem::path::preferred_separator + "public_keys" + std::filesystem::path::preferred_separator;
#endif

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog();



    Identity *get_identity() const;
    void set_identity(Identity *new_identity);

private slots:
    void on_button_browse_clicked();

    void on_buttons_confirm_accepted();

    void on_buttons_confirm_rejected();

private:
    Ui::ExportDialog *ui;
    QString file_name;
    Identity *identity;
    std::filesystem::path public_key_dir = PUBLIC_KEY_HOME;
};

#endif // EXPORTDIALOG_HPP
