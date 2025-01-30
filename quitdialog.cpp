#include "quitdialog.h"
#include "ui_quitdialog.h"

CQuitDialog::CQuitDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CQuitDialog)
{
    ui->setupUi(this);
    // set boarderless and always on top
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    emit QuitDialogIsActive(true);
}

CQuitDialog::~CQuitDialog()
{
    delete ui;
}

void CQuitDialog::on_toolButton_ok_clicked()
{
    logger()<<"User exit, bye bye";
    std::exit(0);
}


void CQuitDialog::on_toolButton_cancel_clicked()
{
    emit QuitDialogIsActive(false);
    this->close();
}

