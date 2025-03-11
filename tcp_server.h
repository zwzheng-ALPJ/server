#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "logger.h"

#include <string.h>
#include <unistd.h>
#include <QObject>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class CTcpServer : public QObject
{
    Q_OBJECT

public:
    CTcpServer();
    int Bind(int target_port);
    int Listen(int max_connect);
    void Accept();
    void Receive();
    int Send(char* message);
    void Close();
    void Reset();

signals:
    void ReceivedData(QString data);
    void ReceivedDataWithIP(QString data,QString ip);

protected:
    int sockfd;
    struct sockaddr_in serveraddr;
    int clientfd;
    char recv_content[512];
    short last_port;
    short last_max_connect;
    bool is_socket_alive=true;
};

#endif // TCP_SERVER_H
