#ifndef COMMUNI_PROTOCOL_HANDLER_H
#define COMMUNI_PROTOCOL_HANDLER_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QtConcurrent>

#include "logger.h"

class CCommuniProtocolHandler : public QObject
{
    Q_OBJECT
public:
    CCommuniProtocolHandler();

    void StoreMMData();

    void StorePPGECGData();

    void StartStoring();

signals:
    void NewDataFromDevice(QString id,qint64 time,QString category,QString data);

    void MMDataToDb(QString id,qint64 time,QString heartrate,QString breathe);

    void PPGECGDataToDb(QString id,qint64 time,QString hr,QString sys,QString dia,QString rr,QString sna,QString qt,QString sdnn,QString fag,QString arr,QString lvet,QString pat,QString sis,QString pwv,QString ptt,QString pep);

    void PPGWaveToDb(QString id,qint64 time,QString wave);

    void ECGWaveToDb(QString id,qint64 time,QString wave);

    void BufferDeviceCount(int num);

    void BufferDeviceId(QStringList sl);

public slots:
    void ProcessRecvedData(QString data);

private:

    QMap<QString, QMap<QString, QString>> mm_radar_buffer;

    QMap<QString, QMap<QString, QString>> ppgecg_buffer;

    const QStringList mm_radar_cat={"MMHR","MMBR"};

    const QStringList ppgecg_cat={"HR","SYS","DIA","RR","SNA","QT","SDNN","FAG","ARR","LVET","PAT","SIS","PWV","PTT","PEP"};

private slots:
    void OnNewDataFromDevice(QString id,qint64 time,QString category,QString data);

};

#endif // COMMUNI_PROTOCOL_HANDLER_H
