#include "sql_connect.h"


CSqlConnect::CSqlConnect()
{
    db = QSqlDatabase::addDatabase("QODBC","mydb");

    timer = new QTimer(this);
    startTime = QTime::currentTime();
    connect(timer, SIGNAL(timeout()), this, SLOT(CheckAlive()));
    timer->start(2000);
}


bool CSqlConnect::Connect(const QString &host, const QString &db_name, const QString &user, const QString &password)
{
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(db_name);
    db.setUserName(user);
    db.setPassword(password);

    if (!db.open())
    {
        logger() << "Can't connect to database:" << db.lastError().text();
        return false;
    }
    logger()<<"Databse connected";
    return true;
}

QStringList CSqlConnect::GetTables()
{
    return db.tables();
}

void CSqlConnect::DriverTest()
{
    QStringList drivers = QSqlDatabase::drivers();
    logger()<<drivers;
}

QSqlQuery CSqlConnect::Exec(QString query)
{
    QSqlQuery q(db);
    return db.exec(query);
}

void CSqlConnect::OnMMDataToDb(QString id,qint64 time,QString heartrate,QString breathe)
{
    QString query=QString("INSERT INTO `MicroMeter` (`DeviceID`, `Time`, `HR`, `BR`) VALUES ('%1', '%2', '%3', '%4');").arg(id).arg(QString::number(time)).arg(heartrate).arg(breathe);
    logger()<<"exec:"<<query;
    Exec(query);
}

void CSqlConnect::OnPPGECGDataToDb(QString id,qint64 time,QString hr,QString sys,QString dia,QString rr,QString sna,QString qt,QString sdnn,QString fag,QString arr,QString lvet,QString pat,QString sis,QString pwv,QString ptt,QString pep)
{
    QString query=QString("INSERT INTO `PPGECGData` (`DeviceID`, `Time`, `HR`, `SYS`, `DIA`, `RR`, `SNA`, `QT`, `SDNN`, `FAG`, `ARR`, `LVET`, `PAT`, `SIS`, `PWV`, `PTT`, `PEP`) VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17');").arg(id).arg(QString::number(time)).arg(hr).arg(sys).arg(dia).arg(rr).arg(sna).arg(qt).arg(sdnn).arg(fag).arg(arr).arg(lvet).arg(pat).arg(sis).arg(pwv).arg(ptt).arg(pep);
    logger()<<"exec:"<<query;
    Exec(query);
}


void CSqlConnect::CheckAlive()
{
    if(db.isOpen())
        emit SQLConnectionStateChange(true);
    else
    {
        emit SQLConnectionStateChange(false);
        Connect("localhost","mydb","root","12345678");
    }

}

