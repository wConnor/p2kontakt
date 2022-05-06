#include "exportdialog.hpp"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::on_button_browse_clicked()
{
    const std::filesystem::path identity_dir = IDENTITY_HOME;
    file_name = QFileDialog::getExistingDirectory(this, "Export Contact File", QString::fromStdString(public_key_dir.string()), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks)
            + std::filesystem::path::preferred_separator + ui->input_filename->text() + ui->input_extension->text();
    ui->input_directory->setText(file_name);
}

Identity *ExportDialog::get_identity() const
{
    return identity;
}

void ExportDialog::set_identity(Identity *new_identity)
{
    identity = new_identity;
}

void ExportDialog::on_buttons_confirm_accepted()
{
    identity->export_contact_file(file_name.toStdString()) ?
                QMessageBox::information(this, "Exported", "Successfully exported contact file to " + file_name + ".") :
                QMessageBox::critical(this, "Error", "Failed to export contact file due to an unknown reason.");
}


void ExportDialog::on_buttons_confirm_rejected()
{
    close();
}

