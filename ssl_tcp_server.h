#ifndef SSL_TCP_SERVER_H
#define SSL_TCP_SERVER_H

#include "tcp_server.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

class CSslTcpServer : public CTcpServer
{
protected:
    typedef enum {
        SSL_MODE_SERVER,
        SSL_MODE_CLIENT
    } SSL_MODE;

public:
    CSslTcpServer();

    SSL* sync_initialize_ssl(const char *cert_path, const char *key_path, SSL_MODE mode, int fd);

    void Accept();

    void Receive();

    int Send(char* message);

    void Close();

private:
    SSL *ssl;

    const char *cert_path="/Users/zzw/学习/UK/Computer Science/A-level project/SSL证书/cert.pem";
    const char *key_path="/Users/zzw/学习/UK/Computer Science/A-level project/SSL证书/key.pem";


};

#endif // SSL_TCP_SERVER_H
