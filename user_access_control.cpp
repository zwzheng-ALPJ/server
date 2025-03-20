#include "user_access_control.h"

CUserAccessControl::CUserAccessControl()
{
    ReadSavedUsers();
    srand((unsigned)time(NULL));
}


bool CUserAccessControl::ReadSavedUsers()
{
    QFile file(json_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logger() << "can't open file: " << file.errorString();
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        logger() << "json prase error: " << jsonError.errorString();
        return false;
    }

    QJsonObject rootObj = document.object();
    if (!rootObj.contains("users")) {
        logger() << "can't fine user key";
        return false;
    }

    QJsonValue usersValue = rootObj.value("users");
    if (!usersValue.isArray()) {
        logger() << "user key type wrong";
        return false;
    }

    QJsonArray usersArray = usersValue.toArray();

    for (int i = 0; i < usersArray.size(); ++i) {
        QJsonObject userObj = usersArray.at(i).toObject();
        QString username = userObj.value("user").toString();
        QString password = userObj.value("pass").toString();

        if (!username.isEmpty() && !password.isEmpty()) {
            user_map.insert(username, password);
        }
    }

    logger() << "Loaded user info:"<<user_map;

    return true;
}


bool CUserAccessControl::SaveUserToJson()
{
    QJsonArray usersArray;
    for (auto it = user_map.constBegin(); it != user_map.constEnd(); ++it)
    {
        QJsonObject userObj;
        userObj.insert("user", it.key());
        userObj.insert("pass", it.value());
        usersArray.append(userObj);
    }

    QJsonObject rootObj;
    rootObj.insert("users", usersArray);

    QJsonDocument doc(rootObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    QFile file(json_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Can't open file: " << file.errorString();
        return false;
    }

    file.write(jsonData);
    file.close();

    logger()<<"User json write success";
    return true;
}

bool CUserAccessControl::Exist(QString username)
{
    return user_map.contains(username);
}

QString CUserAccessControl::Login(QString username,QString password,QString client_ip)
{
    if(!Exist(username))
        return "ERROR: user not found";

    QString token_seed("");
    qint64 CurrentMEpoch = QDateTime::currentMSecsSinceEpoch();
    token_seed.append(QString::number(CurrentMEpoch)).append(client_ip.remove(QRegularExpression("[^0-9]")));

    if (user_map.value(username)==password)
    {
        //unsigned long long randnum=rand()%ULLONG_MAX;
        unsigned long long randnum = QRandomGenerator::global()->generate64();
        //QRandomGenerator seems to have a stronger randomness when generating numbers, which is safer in this case

        token_seed.append(QString::number(randnum));

        //logger()<<token_seed;

        QByteArray ba=token_seed.toUtf8();
        QByteArray token_ba = QCryptographicHash::hash(ba, QCryptographicHash::Md5);
        QString token=token_ba.toHex();

        token_map.insert(token, qMakePair(qMakePair(username, client_ip), qMakePair(QDateTime::currentDateTime(),QDateTime::currentDateTime())));
        return token;
    }
    else
        return "ERROR: wrong password";
}


bool CUserAccessControl::IsTokenValid(QString token)
{
    if(!token_map.contains(token))
        return false;

    QDateTime time_now = QDateTime::currentDateTime();
    QDateTime token_create_time=token_map.value(token).second.first;
    QDateTime token_last_used_time=token_map.value(token).second.second;

    if(token_create_time.secsTo(time_now)>TOKEN_LIFE_SINCE_CREATED || token_last_used_time.secsTo(time_now)>TOKEN_LIFE_SINCE_LAST_USED)
    {
        token_map.remove(token);
        return false;
    }

    token_map[token].second.second=time_now;

    return true;
}

