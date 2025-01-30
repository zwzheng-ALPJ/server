#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QtConcurrent>
#include <QTime>
#include <QCloseEvent>

#include "tcp_server.h"
#include "ssl_tcp_server.h"
#include "sql_connect.h"
#include "logger.h"
#include "communi_protocol_handler.h"
#include "quitdialog.h"
#include "ssl_tcp_server.h"
#include "mobile_communi_protocol_handler.h"
#include "user_access_control.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent*);


private:
    Ui::MainWindow *ui;

    CTcpServer tcp_s;

    CTcpServer ssl_tcp_s;

    CCommuniProtocolHandler server;

    CMobileCommuniProtocolHandler mobile;

    CSqlConnect db;

    void InitSocket();

    void StartSocketUpdate();

    void StartSSLSocketUpdate();

    void InitSslSocket();

    QTimer *timer;
    QTime startTime;

    CQuitDialog *dialog = new CQuitDialog(this);
    bool quit_dialog_is_active=false;

private slots:
    void UpdateRunTime();

    void UpdateDeviceCount(int num);

    void OnSQLConnectionStateChanges(bool connected);

    void UpdateDeviceList(QStringList sl);

    void OnQuitDialogIsActive(bool state);

    void on_toolButton_bind_clicked();
    void on_toolButton_listen_clicked();
    void on_toolButton_accept_clicked();
    void on_toolButton_recv_clicked();
    void on_toolButton_home_clicked();
    void on_toolButton_data_clicked();
    void on_toolButton_data_2_clicked();
    void on_toolButton_setting_clicked();
    void on_toolButton_quit_clicked();
    void on_toolButton_ssl_bind_clicked();
    void on_toolButton_ssl_listen_clicked();
    void on_toolButton_ssl_accept_clicked();
    void on_toolButton_ssl_recv_clicked();
    void on_toolButton_ssl_close_clicked();
    void on_toolButton_ssl_send_clicked();
    void on_toolButton_user_save_clicked();
};
#endif // MAINWINDOW_H
