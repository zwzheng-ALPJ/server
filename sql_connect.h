#ifndef SQL_CONNECT_H
#define SQL_CONNECT_H

#include <QObject>
#include <QtCore/qtimer.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include <mysql.h>

#include "logger.h"

class CSqlConnect : public QObject
{
    Q_OBJECT

public:
    CSqlConnect();

    bool Connect(const QString &host, const QString &db_name, const QString &user, const QString &password);

    void DriverTest();

    QStringList GetTables();

    QSqlQuery Exec(QString query);

public slots:
    void OnMMDataToDb(QString id,qint64 time,QString heartrate,QString breathe);

    void OnPPGECGDataToDb(QString id,qint64 time,QString hr,QString sys,QString dia,QString rr,QString sna,QString qt,QString sdnn,QString fag,QString arr,QString lvet,QString pat,QString sis,QString pwv,QString ptt,QString pep);

    void CheckAlive();

signals:
    void SQLConnectionStateChange(bool connected);

private:
    QSqlDatabase db;
    const int port=3306;

    QTimer *timer;
    QTime startTime;
};

#endif // SQL_CONNECT_H
