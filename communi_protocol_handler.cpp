#include "communi_protocol_handler.h"

CCommuniProtocolHandler::CCommuniProtocolHandler()
{
    connect(this,SIGNAL(NewDataFromDevice(QString,qint64,QString,QString)),this,SLOT(OnNewDataFromDevice(QString,qint64,QString,QString)));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CCommuniProtocolHandler::StartStoring);
    timer->start(2000);
}

void CCommuniProtocolHandler::StartStoring()
{
    QtConcurrent::run([this]{StoreMMData();});
    QtConcurrent::run([this]{StorePPGECGData();});
}

void CCommuniProtocolHandler::ProcessRecvedData(QString data)
{
    QString start = "START";
    QString end = "END";

    if (data.startsWith(start) && data.endsWith(end))
        data = data.mid(start.length(), data.length() - start.length() - end.length());
    else
        return;
    //logger()<<"Processed data:"<<data;
    QStringList sl=data.split(",");

    QStringList id=sl[0].split("=");
    QStringList content=sl[1].split("=");
    //logger()<<id[1]<<content[0]<<content[1];
    qint64 timenow=QDateTime::currentMSecsSinceEpoch();
    emit NewDataFromDevice(id[1],timenow,content[0],content[1]);
}

void CCommuniProtocolHandler::OnNewDataFromDevice(QString id,qint64 time,QString category,QString data)
{
    if (mm_radar_cat.contains(category))
    {
        mm_radar_buffer[id][category]=data;
    }
    else if (ppgecg_cat.contains(category))
    {
        ppgecg_buffer[id][category]=data;
        logger()<<"stored"<<category<<data;
    }
    else if(category=="PPGW")
    {
        emit PPGWaveToDb(id,time,data);
    }
    else if(category=="ECGW")
    {
        emit ECGWaveToDb(id,time,data);
    }
}

void CCommuniProtocolHandler::StoreMMData()
{
    int device_counter=0;
    QStringList device_list;
    qint64 timenow = QDateTime::currentMSecsSinceEpoch();
    for (auto it = mm_radar_buffer.begin(); it != mm_radar_buffer.end(); ++it)
    {
        QString id = it.key();
        QMap<QString, QString>& categories = it.value();

        QStringList data;
        for (int i=0;i<mm_radar_cat.size();i++)
            data << QString();

        for(int i=0;i<mm_radar_cat.size();i++)
        {
            if (categories.contains(mm_radar_cat[i]))
            {
                data[i]=categories[mm_radar_cat[i]];
            }
        }
        logger()<<"got data from buffer:"<<data;
        if (!data[0].isEmpty() || !data[1].isEmpty())
        {
            emit MMDataToDb(id, timenow, data[0], data[1]);
        }

        device_counter++;
        device_list<<id;
    }

    emit BufferDeviceCount(device_counter);
    emit BufferDeviceId(device_list);
    mm_radar_buffer.clear();
}

bool areAllItemsNonEmpty(const QStringList &list) {
    for (const QString &item : list) {
        if (item.isEmpty()) {
            return false;
        }
    }
    return true;
}

void CCommuniProtocolHandler::StorePPGECGData()
{
    qint64 timenow = QDateTime::currentMSecsSinceEpoch();
    for (auto it = ppgecg_buffer.begin(); it != ppgecg_buffer.end(); ++it)
    {
        QString id = it.key();
        QMap<QString, QString>& categories = it.value();

        QStringList data;
        for (int i=0;i<ppgecg_cat.size();i++)
            data << QString();

        for(int i=0;i<ppgecg_cat.size();i++)
        {
            if (categories.contains(ppgecg_cat[i]))
            {
                data[i]=categories[ppgecg_cat[i]];
            }
        }
        logger()<<"got data from buffer:"<<data;
        if (areAllItemsNonEmpty(data))
        {
            emit PPGECGDataToDb(id, timenow, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], data[14]);
        }
    }
    ppgecg_buffer.clear();
}
