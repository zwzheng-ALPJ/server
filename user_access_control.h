#ifndef USER_ACCESS_CONTROL_H
#define USER_ACCESS_CONTROL_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <limits.h>
#include <QCryptographicHash>
#include <QRandomGenerator>

#include "logger.h"

#define TOKEN_LIFE_SINCE_CREATED 72*3600
#define TOKEN_LIFE_SINCE_LAST_USED 12*3600

class CUserAccessControl : public QObject
{
    Q_OBJECT
public:

    CUserAccessControl();

    bool Exist(QString username);

    QString Login(QString username,QString password,QString client_ip);

    bool IsTokenValid(QString token);

public slots:

    bool SaveUserToJson();


private:

    QMap<QString, QString> user_map;

    QMap<QString, QPair<QPair<QString, QString>, QPair<QDateTime, QDateTime>>> token_map;
    // [token,[username, client_ip],[create time, last used time]]

    const QString json_path="/Users/zzw/QtP/CS-project-server-final/users.json";

    bool ReadSavedUsers();
};

#endif // USER_ACCESS_CONTROL_H
