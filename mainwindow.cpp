#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);


    //####################################################
    // -------------------Database----------------------##
    //####################################################

    if(db.Connect("localhost","mydb","root","12345678"))
    {
        OnSQLConnectionStateChanges(true);
    }
    connect(&server,SIGNAL(MMDataToDb(QString,qint64,QString,QString)),&db,SLOT(OnMMDataToDb(QString,qint64,QString,QString)));
    connect(&server,SIGNAL(PPGECGDataToDb(QString,qint64,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)),&db,SLOT(OnPPGECGDataToDb(QString,qint64,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));


    //####################################################
    // --------------------Socket Server----------------##
    //####################################################

    bool connected_raspi_slot=connect(&tcp_s,SIGNAL(ReceivedData(QString)),&server,SLOT(ProcessRecvedData(QString)));
    bool connected_mobile_slot=connect(&ssl_tcp_s,SIGNAL(ReceivedDataWithIP(QString,QString)),&mobile,SLOT(ProcessRecvedData(QString,QString)));

    logger()<<"树莓派的通信槽函数连接了吗？："<<connected_raspi_slot;
    logger()<<"手机的通信槽函数连接了吗？："<<connected_mobile_slot;
    InitSocket();
    InitSslSocket();
    QtConcurrent::run([this]{StartSocketUpdate();});
    QtConcurrent::run([this]{StartSSLSocketUpdate();});


    //####################################################
    // -----------------------UI------------------------##
    //####################################################

    // initialize timer
    timer = new QTimer(this);
    startTime = QTime::currentTime();
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateRunTime()));
    timer->start(1000);

    // status track
    connect(&server, SIGNAL(BufferDeviceCount(int)), this, SLOT(UpdateDeviceCount(int)));
    connect(&db,SIGNAL(SQLConnectionStateChange(bool)),this,SLOT(OnSQLConnectionStateChanges(bool)));
    connect(&server,SIGNAL(BufferDeviceId(QStringList)),this,SLOT(UpdateDeviceList(QStringList)));

    // device live data display
    ResetLiveData();
    connect(this,SIGNAL(MonitorDeviceChanged(QString)),this,SLOT(on_MonitorDeviceChanged(QString)));
    connect(&server,SIGNAL(MMDataToDb(QString,qint64,QString,QString)),this,SLOT(on_NewMMData(QString,qint64,QString,QString)));
    connect(&server,SIGNAL(PPGECGDataToDb(QString,qint64,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)),this,SLOT(on_NewPPGECGData(QString,qint64,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));

    // quit dialogue init
    connect(dialog,SIGNAL(QuitDialogIsActive(bool)),this,SLOT(OnQuitDialogIsActive(bool)));

    // set initial page

    ui->stackedWidget->setCurrentIndex(0);


    //####################################################
    // -----------------Debugging stuff-----------------##
    //####################################################

    // 窗口的宽度和高度(含边框)
    // qDebug() << this->frameGeometry().width() << this->frameGeometry().height();


    // 窗口的宽度和高度(不含边框)
    // qDebug() << this->geometry().width() << this->geometry().height();//1
    // qDebug() << this->width() << this->height();//2
    // qDebug() << this->rect().width() << this->rect().height();//3
    // qDebug() << this->size().width() << this->size().height();//4

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    e->ignore();
    this->showMinimized();
}

void MainWindow::InitSocket()
{
    if(tcp_s.Bind(7777)<0)
    {
        logger()<<"bind error";
        QMessageBox::StandardButton msgbox;
        msgbox=QMessageBox::critical(this,tr("Error"),"Failed to start socket server",QMessageBox::Retry|QMessageBox::Cancel);
        if(msgbox==QMessageBox::Retry)
        {
            std::exit(-1);
        }
        else if(msgbox==QMessageBox::Cancel)
            std::exit(-1);
    }
    else
    {
        logger()<<"server bind success on port 7777";
    }
    tcp_s.Listen(10);
    logger()<<"Socket Init END";
}

void MainWindow::StartSocketUpdate()
{
    logger()<<"Start keep updating socket...";
    while(1)
    {
        tcp_s.Accept();
        tcp_s.Receive();
    }
}

void MainWindow::StartSSLSocketUpdate()
{
    while(!ssl_socket_stop_flag)
    {
        ssl_tcp_s.Accept();
        ssl_tcp_s.Receive();
    }
}

void MainWindow::InitSslSocket()
{
    if(ssl_tcp_s.Bind(7778)<0)
    {
        logger()<<"ssl tcp server bind error";
        QMessageBox::StandardButton msgbox;
        msgbox=QMessageBox::critical(this,tr("Error"),"Failed to start SSL socket server",QMessageBox::Retry|QMessageBox::Cancel);
        if(msgbox==QMessageBox::Retry)
        {
            std::exit(-1);
        }
        else if(msgbox==QMessageBox::Cancel)
            std::exit(-1);
    }
    else
    {
        logger()<<"SSL server bind success on port 7778";
    }
    ssl_tcp_s.Listen(1);
}

void MainWindow::SetSSLSocketStop(bool state)
{
    if(state)
        ssl_socket_stop_flag=false;
    else
    {
        ssl_socket_stop_flag=true;
        ssl_tcp_s.Close();
    }
}

void MainWindow::UpdateRunTime()
{
    int elapsed = startTime.secsTo(QTime::currentTime());
    QTime runTime(0, 0);
    runTime = runTime.addSecs(elapsed);
    ui->label_run_time->setText(runTime.toString("hh:mm:ss"));
}

void MainWindow::UpdateDeviceCount(int num)
{
    // logger()<<"set UI device count to"<<num;
    ui->label_connections->setText(QString::number(num));
    ui->label_home_device_count->setText(QString::number(num));
    if(num>0)
    {
        ui->label_home_device_count->setStyleSheet("color:#60D838");
        ui->label_online_check->setStyleSheet("image: url(:/navi/green_circle.svg);");
    }
    else
    {
        ui->label_home_device_count->setStyleSheet("color:#FF634D");
        ui->label_online_check->setStyleSheet("image: url(:/navi/red_circle.svg);");
    }
}

void MainWindow::OnSQLConnectionStateChanges(bool connected)
{
    if(connected)
    {
        ui->label_SQL->setStyleSheet("image: url(:/navi/sql_green.png);");
        ui->label_sql_check->setStyleSheet("image: url(:/navi/green_circle.svg);");
        ui->label_home_sql_stat->setText("connected");
        ui->label_home_sql_stat->setStyleSheet("color:#60D838");
    }
    else
    {
        ui->label_SQL->setStyleSheet("image: url(:/navi/sql_amber.png);");
        ui->label_sql_check->setStyleSheet("image: url(:/navi/red_circle.svg);");
        ui->label_home_sql_stat->setText("not connected, you may want to restart the program");
        ui->label_home_sql_stat->setStyleSheet("color:#FF634D");
    }
}

void MainWindow::UpdateDeviceList(QStringList sl)
{
    QStringListModel *model;
    model = new QStringListModel(this);
    model->setStringList(sl);
    ui->listView_devices->setModel(model);
}


void MainWindow::on_toolButton_bind_clicked()
{
    if(tcp_s.Bind(7777)<0)
    {
        logger()<<"bind error";
    }
    else
        logger()<<"bind success on port 7777";
}


void MainWindow::on_toolButton_listen_clicked()
{
    tcp_s.Listen(1);
}


void MainWindow::on_toolButton_accept_clicked()
{
    tcp_s.Accept();
}


void MainWindow::on_toolButton_recv_clicked()
{
    tcp_s.Receive();
}


void MainWindow::on_toolButton_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_toolButton_data_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_toolButton_data_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_toolButton_setting_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_toolButton_quit_clicked()
{
    // prevent showing multiple dialogs
    if(!quit_dialog_is_active)
    {
        dialog->show();
    }
}

void MainWindow::OnQuitDialogIsActive(bool state)
{
    quit_dialog_is_active=state;
}


void MainWindow::on_toolButton_ssl_bind_clicked()
{
    if(ssl_tcp_s.Bind(7778)<0)
    {
        logger()<<"ssl socket bind error";
    }
    else
        logger()<<"ssl socket bind success on port 7778";
}


void MainWindow::on_toolButton_ssl_listen_clicked()
{
    ssl_tcp_s.Listen(1);
}


void MainWindow::on_toolButton_ssl_accept_clicked()
{
    ssl_tcp_s.Accept();
}


void MainWindow::on_toolButton_ssl_recv_clicked()
{
    ssl_tcp_s.Receive();
}


void MainWindow::on_toolButton_ssl_close_clicked()
{
    ssl_tcp_s.Close();
}


void MainWindow::on_toolButton_ssl_send_clicked()
{
    QByteArray ba=ui->textEdit_ssl_send->toPlainText().toLatin1();
    ssl_tcp_s.Send(ba.data());
}



void MainWindow::on_toolButton_user_save_clicked()
{
    mobile.user.SaveUserToJson();
}


void MainWindow::on_listView_devices_clicked(const QModelIndex &index)
{
    emit MonitorDeviceChanged(index.data(Qt::DisplayRole).toString());
}

void MainWindow::on_MonitorDeviceChanged(QString new_device_id)
{
    if(monitered_device!=new_device_id)
    {
        ResetLiveData();
        monitered_device=new_device_id;
        logger() << "Changed monitoring target:" << monitered_device;
        ui->label_live_data->setText("Live Data from "+monitered_device);
    }
}

void MainWindow::on_NewMMData(QString id,qint64 time,QString heartrate,QString breathe)
{
    if(id==monitered_device)
    {
        ui->label_heart_rate_data->setText(heartrate);
        ui->label_breathe_data->setText(breathe);
    }
}

void MainWindow::on_NewPPGECGData(QString id,qint64 time,QString hr,QString sys,QString dia,QString rr,QString sna,QString qt,QString sdnn,QString fag,QString arr,QString lvet,QString pat,QString sis,QString pwv,QString ptt,QString pep)
{
    if(id==monitered_device)
    {
        logger()<<"update ecg data on ui";
        ui->label_heart_rate_data->setText(hr);
        ui->label_breathe_data->setText(rr);

        ui->label_sys_data->setText(sys);
        ui->label_dia_data->setText(dia);
        ui->label_sna_data->setText(sna);
        ui->label_qt_data->setText(qt);
        ui->label_sdnn_data->setText(sdnn);
        ui->label_fag_data->setText(fag);
        ui->label_arr_data->setText((arr=="0")?"Not Detected":"Detected!");
        ui->label_lvet_data->setText(lvet);
        ui->label_pat_data->setText(pat);
        ui->label_sis_data->setText(sis);
        ui->label_pwv_data->setText(pwv);
        ui->label_ptt_data->setText(ptt);
        ui->label_pep_data->setText(pep);
    }
}

void MainWindow::ResetLiveData()
{
    ui->label_live_data->setText("Choose a device from the list...");
    ui->label_heart_rate_data->setText("--");
    ui->label_breathe_data->setText("--");
    ui->label_sys_data->setText("--");
    ui->label_dia_data->setText("--");
    ui->label_sna_data->setText("--");
    ui->label_qt_data->setText("--");
    ui->label_sdnn_data->setText("--");
    ui->label_fag_data->setText("--");
    ui->label_arr_data->setText("--");
    ui->label_lvet_data->setText("--");
    ui->label_pat_data->setText("--");
    ui->label_sis_data->setText("--");
    ui->label_pwv_data->setText("--");
    ui->label_ptt_data->setText("--");
    ui->label_pep_data->setText("--");
}
