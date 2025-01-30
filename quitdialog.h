#ifndef QUITDIALOG_H
#define QUITDIALOG_H

#include <QDialog>
#include "logger.h"

namespace Ui {
class CQuitDialog;
}

class CQuitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CQuitDialog(QWidget *parent = nullptr);
    ~CQuitDialog();

private slots:
    void on_toolButton_ok_clicked();

    void on_toolButton_cancel_clicked();

private:
    Ui::CQuitDialog *ui;

signals:
    void QuitDialogIsActive(bool state);
};

#endif // QUITDIALOG_H
