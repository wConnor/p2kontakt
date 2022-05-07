#include "mainwindow.hpp"
#include "identitydialog.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <iostream>
#include <filesystem>

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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList ui_languages = QLocale::system().uiLanguages();

    const std::filesystem::path identity_dir = IDENTITY_HOME;
    const std::filesystem::path public_key_dir = PUBLIC_KEY_HOME;

    /* An inexistent or empty identity directory implies that there are no identities on
     * the local system. New identity form is ran upon the first use of the program. */
    if (!std::filesystem::exists(identity_dir) || std::filesystem::is_empty(identity_dir)
            || !std::filesystem::exists(public_key_dir)) {
        std::filesystem::create_directories(identity_dir);
        std::filesystem::create_directories(public_key_dir);
        IdentityDialog identity_dialog;
        identity_dialog.set_first_time(true);
        identity_dialog.setModal(true);
        identity_dialog.exec();
    }

    MainWindow mw;
    mw.show();

    for (const QString &locale : ui_languages) {
        const QString baseName = "p2kontakt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    return a.exec();
}
