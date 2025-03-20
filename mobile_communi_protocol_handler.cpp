#include "mobile_communi_protocol_handler.h"

CMobileCommuniProtocolHandler::CMobileCommuniProtocolHandler() {}

void CMobileCommuniProtocolHandler::ProcessRecvedData(QString data,QString client_ip)
{
    logger()<<"mobile socket handler triggered";
    QString start = "START";
    QString end = "END";

    QString login="LOGIN";

    if (data.startsWith(start) && data.endsWith(end))
    {
        data = data.mid(start.length(), data.length() - start.length() - end.length());
        if (data.startsWith(login))
        {
            data=data.mid(login.length(),data.length()-login.length());
            logger()<<"Login request: "<<data;
            QStringList sl=data.split(":");
            QString token=user.Login(sl[0],sl[1],client_ip);
            logger()<<token;
            if(!token.isEmpty())
            {

            }
            return;
        }
    }
    else
        return;

    //logger()<<"Processed data:"<<data;
    QStringList sl=data.split(",");

    QStringList id=sl[0].split("=");
    QStringList content=sl[1].split("=");

    //logger()<<id[1]<<content[0]<<content[1];
    qint64 timenow=QDateTime::currentMSecsSinceEpoch();
    // emit NewDataFromDevice(id[1],timenow,content[0],ontent[1]);
}
