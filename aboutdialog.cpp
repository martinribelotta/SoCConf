#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->buttonBox->addButton(tr("Close"), QDialogButtonBox::AcceptRole);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
