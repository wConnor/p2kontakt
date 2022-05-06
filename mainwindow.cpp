#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->list_tabs->addItem("-> Main");
    ui->text_chat->setVisible(false);
    ui->input_chat->setVisible(false);
    ui->button_send->setVisible(false);
    ui->label_chatting->setVisible(false);
    ui->label_server_status->setVisible(false);
}

void MainWindow::on_action_new_identity_triggered()
{
    IdentityDialog identity_dialog;
    identity_dialog.setModal(true);
    identity_dialog.exec();
}

void MainWindow::on_action_about_triggered()
{
    AboutWindow *about_window = new AboutWindow();
    about_window->show();
}

void MainWindow::on_button_change_identity_clicked()
{
    const std::filesystem::path identity_dir = IDENTITY_HOME;
    QString file_name = QFileDialog::getOpenFileName(this, "Select Identity File", QString::fromStdString(identity_dir.string()));
    QFile file(file_name);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Warning", "Unable to open identity file: " + file.errorString());
    } else {
        std::unique_ptr<Identity> loaded_identity = std::make_unique<Identity>(file);
        this->identity = *loaded_identity;
        setWindowTitle("p2kontakt: " + QString::fromStdString(identity.get_userid()));
        ui->label_userid->setText(QString::fromStdString(identity.get_userid()));
        ui->label_fingerprint->setText(identity.get_key()->fpr);

        populate_contacts();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populate_contacts()
{
    ui->list_tabs->clear();
    ui->list_tabs->addItem("-> Main");

    auto contacts_list = identity.get_contacts_list();

    for (auto &c : contacts_list) {
        ui->list_tabs->addItem(QString::fromStdString(identity.resolve_name(c.first)));
    }
}

void MainWindow::on_action_encrypt_triggered()
{
    if (identity.get_userid().empty()) {
        QMessageBox::critical(this, "Error", "Unable to open the encryption tool as no identity has been loaded.");
    } else {
        EncryptDialog *encrypt_dialog = new EncryptDialog();
        encrypt_dialog->set_identity(&this->identity);
        encrypt_dialog->show();
    }
}

void MainWindow::on_action_decrypt_triggered()
{
    if (identity.get_userid().empty()) {
        QMessageBox::critical(this, "Error", "Unable to open the decryption tool as no identity has been loaded.");
    } else {
        DecryptDialog *decrypt_dialog = new DecryptDialog();
        decrypt_dialog->set_identity(&this->identity);
        decrypt_dialog->show();
    }
}

const Identity &MainWindow::get_identity() const
{
    return identity;
}

void MainWindow::set_identity(const Identity &new_identity)
{
    identity = new_identity;
}

void MainWindow::on_action_export_contact_file_triggered()
{
    if (identity.get_userid().empty()) {
        QMessageBox::critical(this, "Error", "Unable to export the contact file as an identity has not been loaded.");
    } else {
        ExportDialog export_dialog;
        export_dialog.setModal(true);
        export_dialog.set_identity(&identity);
        export_dialog.exec();
    }
}


void MainWindow::on_action_add_contact_triggered()
{
    if (identity.get_userid().empty()) {
        QMessageBox::critical(this, "Error", "Unable to add a contact as an identity has not been loaded.");
    } else {
        AddContactDialog add_contact_dialog;
        add_contact_dialog.setModal(true);
        add_contact_dialog.set_identity(&identity);
        add_contact_dialog.exec();
        populate_contacts();
    }
}


void MainWindow::on_action_authorise_ip_triggered()
{
    if (identity.get_userid().empty()) {
        QMessageBox::critical(this, "Error", "Unable to authorise an IP as an identity has not been loaded.");
    } else {
        AuthoriseIPDialog authorise_ip_dialog;
        authorise_ip_dialog.setModal(true);
        authorise_ip_dialog.set_identity(&identity);
        authorise_ip_dialog.exec();
    }
}


void MainWindow::on_action_delete_identity_triggered()
{
    if (identity.get_userid().empty()) {
        QMessageBox::critical(this, "Error", "Unable to delete the current identity as an identity has not been loaded.");
    } else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Identity", "Are you sure you would like to delete the currently loaded identity?\n"
                                                               "There will be no way to recover your identity afterwards.",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            if (identity.delete_identity()) {
                QMessageBox::information(this, "Success", "Identity file has successfully been deleted.");
            } else {
                QMessageBox::critical(this, "Failure", "Failed to delete identity. You may not have permission to remove files in the identities directory.");
            }
        }
    }
}


void MainWindow::on_list_tabs_itemDoubleClicked(QListWidgetItem *item)
{
    if (item->text() == "-> Main") {
        ui->text_chat->setVisible(false);
        ui->input_chat->setVisible(false);
        ui->button_send->setVisible(false);
        ui->label_chatting->setVisible(false);
        ui->label_server_status->setVisible(false);
    } else {
        std::string selected_fpr = identity.resolve_fingerprint(ui->list_tabs->currentItem()->text().toStdString());
        ui->text_chat->clear();
        ui->text_chat->setVisible(true);
        ui->input_chat->setVisible(true);
        ui->button_send->setVisible(true);
        ui->label_chatting->setVisible(true);
        ui->label_chatting->setText("Chatting with " + item->text() + " (" + QString::fromStdString(selected_fpr) + ")");
        ui->label_server_status->setVisible(true);
        this->identity.prepare_connection(*ui->label_chatting, *ui->label_server_status,
                                          selected_fpr, *ui->text_chat);
    }
}

void MainWindow::on_button_send_clicked()
{
    if (ui->input_chat->text() != "") {
        std::string message = ui->input_chat->text().toStdString();

        if (identity.send_message(message)) {
            ui->text_chat->moveCursor(QTextCursor::End);
            ui->text_chat->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " - <");
            ui->text_chat->moveCursor(QTextCursor::End);
            ui->text_chat->setTextColor(Qt::red);
            ui->text_chat->insertPlainText(QString::fromStdString(identity.get_userid()));
            ui->text_chat->moveCursor(QTextCursor::End);
            ui->text_chat->setTextColor(Qt::black);
            ui->text_chat->insertPlainText(">: " + QString::fromStdString(message) + '\n');
        } else {
            ui->text_chat->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " - Failed to send message.");
        }

        ui->input_chat->clear();
        // send message
    }
}


void MainWindow::on_action_reset_triggered()
{
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Reset", "Are you sure would like to reset all data?\n"
                                         "All identities, including any imported contacts, will be deleted.\n"
                                         "This process is irreversible.");

    if (answer == QMessageBox::Yes) {
        std::filesystem::path identity_dir = IDENTITY_HOME;
        std::filesystem::path contacts_dir = PUBLIC_KEY_HOME;

        std::filesystem::remove_all(identity_dir) && std::filesystem::remove_all(contacts_dir) ?
            QMessageBox::information(this, "Reset", "All data has been succesfully removed.\n"
                                                    "You will need to create a new identity to use p2kontakt.") :
            QMessageBox::critical(this, "Reset", "Failed to reset all data. This may be due to a permissions error on the target directory.");
    }
}


void MainWindow::on_action_clear_chat_triggered()
{
    ui->text_chat->clear();
}

