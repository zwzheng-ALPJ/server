#ifndef MOBILE_COMMUNI_PROTOCOL_HANDLER_H
#define MOBILE_COMMUNI_PROTOCOL_HANDLER_H

#include <QObject>

#include "logger.h"
#include "user_access_control.h"

class CMobileCommuniProtocolHandler : public QObject
{
    Q_OBJECT
public:

    CMobileCommuniProtocolHandler();

    CUserAccessControl user;

public slots:

    void ProcessRecvedData(QString data,QString client_ip);

};

#endif // MOBILE_COMMUNI_PROTOCOL_HANDLER_H
