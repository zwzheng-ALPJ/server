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

    bool ssl_socket_stop_flag=false;

    void InitSslSocket();

    QTimer *timer;
    QTime startTime;

    CQuitDialog *dialog = new CQuitDialog(this);
    bool quit_dialog_is_active=false;

    QString monitered_device;

    void ResetLiveData();

private slots:
    void UpdateRunTime();

    void UpdateDeviceCount(int num);

    void OnSQLConnectionStateChanges(bool connected);

    void UpdateDeviceList(QStringList sl);

    void OnQuitDialogIsActive(bool state);

    void SetSSLSocketStop(bool state);

    void on_MonitorDeviceChanged(QString new_device_id);

    //for UI update only
    void on_NewMMData(QString id,qint64 time,QString heartrate,QString breathe);
    void on_NewPPGECGData(QString id,qint64 time,QString hr,QString sys,QString dia,QString rr,QString sna,QString qt,QString sdnn,QString fag,QString arr,QString lvet,QString pat,QString sis,QString pwv,QString ptt,QString pep);

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
    void on_listView_devices_clicked(const QModelIndex &index);

signals:
    void MonitorDeviceChanged(QString new_device_id);
};
#endif // MAINWINDOW_H
