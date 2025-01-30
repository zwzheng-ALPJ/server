#include "ssl_tcp_server.h"

CSslTcpServer::CSslTcpServer() {}

SSL* CSslTcpServer::sync_initialize_ssl(const char* cert_path, const char* key_path, SSL_MODE mode, int fd)
{
    logger()<<"server::sync_init_ssl";
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl = NULL;
    // Init openssl
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    if (mode == SSL_MODE_SERVER)
    {
        method = SSLv23_server_method();
    }
    else if (mode == SSL_MODE_CLIENT)
    {
        method = SSLv23_client_method();
    }
    else
    {
        printf("unknown method");
        return NULL;
    }

    // create ssl context
    ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        printf("Unable to create SSL context");
        return NULL;
    }

    // configure ssl context
    if (SSL_CTX_use_certificate_file(ctx, cert_path, SSL_FILETYPE_PEM) <= 0 || SSL_CTX_use_PrivateKey_file(ctx, key_path, SSL_FILETYPE_PEM) <= 0 )
    {
        printf("Not found certificate or private key");
        SSL_CTX_free(ctx);
        return NULL;
    }
    // SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

    // create new ssl object
    ssl = SSL_new(ctx);
    if (!ssl)
    {
        printf("Failed to create SSL object.");
        return NULL;
    }

    // set file descriptor
    if (SSL_set_fd(ssl, fd) == 0)
    {
        printf("Failed to set fd to SSL object.");
        return NULL;
    }

    // ssl handshake
    if ((mode == SSL_MODE_CLIENT && SSL_connect(ssl) <= 0) ||
        (mode == SSL_MODE_SERVER && SSL_accept(ssl) <= 0))
    {
        int ssl_result;
        if (mode == SSL_MODE_CLIENT)
        {
            ssl_result = SSL_connect(ssl);
        }
        else if (mode == SSL_MODE_SERVER) {
            ssl_result = SSL_accept(ssl);
        }

        int ssl_err = SSL_get_error(ssl, ssl_result);

        const char *err_str;
        switch (ssl_err)
        {
        case SSL_ERROR_NONE:
            err_str = "No error";
            break;
        case SSL_ERROR_SSL:
            err_str = "Error in the SSL protocol";
            break;
        case SSL_ERROR_WANT_READ:
            err_str = "SSL read operation did not complete";
            break;
        case SSL_ERROR_WANT_WRITE:
            err_str = "SSL write operation did not complete";
            break;
        case SSL_ERROR_WANT_X509_LOOKUP:
            err_str = "SSL X509 lookup operation did not complete";
            break;
        case SSL_ERROR_SYSCALL:
            err_str = "Syscall error";
            break;
        case SSL_ERROR_ZERO_RETURN:
            err_str = "SSL connection was shut down cleanly";
            break;
        case SSL_ERROR_WANT_CONNECT:
            err_str = "SSL connect operation did not complete";
            break;
        case SSL_ERROR_WANT_ACCEPT:
            err_str = "SSL accept operation did not complete";
            break;
        default:
            err_str = "Unknown error";
            break;
        }
        logger()<<"SSL handshake failed. Error: "<<(err_str?err_str:"Unknown");
        return NULL;
    }
    return ssl;
}

void CSslTcpServer::Accept()
{
    logger()<<"server::accept()";
    CTcpServer::Accept();
    ssl=sync_initialize_ssl(cert_path,key_path,SSL_MODE_SERVER,clientfd);
}

void CSslTcpServer::Receive()
{
    logger()<<"server::receive()";

    QString acc_data;

    while(1)
    {
        memset(recv_content, 0, sizeof(recv_content));
        int recv_count = SSL_read(ssl, recv_content, sizeof(recv_content));
        if(recv_count <= 0)
        {
            int ssl_err = SSL_get_error(ssl, recv_count);
            if(ssl_err == SSL_ERROR_ZERO_RETURN || ssl_err == SSL_ERROR_SYSCALL)
            {
                Close();
                break;
            }
            else
            {
                logger() << "SSL socket read error: " << ssl_err;
                break;
            }
        }
        acc_data += QString::fromUtf8(recv_content, recv_count);
    }
    logger() << "SSL socket received data: " << acc_data;

    emit ReceivedData(acc_data);
}

int CSslTcpServer::Send(char* message)
{
    return SSL_write(ssl,message,strlen(message));
}

void CSslTcpServer::Close()
{
    CTcpServer::Close();
    SSL_shutdown(ssl);
    SSL_free(ssl);
}
