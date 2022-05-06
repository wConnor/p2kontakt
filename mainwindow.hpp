#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QFileDevice>
#include <QListWidget>
#include <QListWidgetItem>

#include "exportdialog.hpp"
#include "identitydialog.hpp"
#include "authoriseipdialog.hpp"
#include "addcontactdialog.hpp"
#include "encryptdialog.hpp"
#include "decryptdialog.hpp"
#include "aboutwindow.hpp"

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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void populate_contacts();

    const Identity &get_identity() const;
    void set_identity(const Identity &new_identity);

private slots:
    void on_action_new_identity_triggered();
    void on_action_about_triggered();
    void on_button_change_identity_clicked();
    void on_action_encrypt_triggered();
    void on_action_decrypt_triggered();
    void on_action_export_contact_file_triggered();
    void on_action_add_contact_triggered();
    void on_action_authorise_ip_triggered();
    void on_action_delete_identity_triggered();
    void on_list_tabs_itemDoubleClicked(QListWidgetItem *item);
    void on_button_send_clicked();
    void on_action_reset_triggered();

    void on_action_clear_chat_triggered();

private:
    Ui::MainWindow *ui;
    QFile current_identity_file;
    Identity identity;
};
#endif // MAINWINDOW_HPP
