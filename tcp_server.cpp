#include "tcp_server.h"

CTcpServer::CTcpServer()
{
    sockfd=socket(AF_INET,SOCK_STREAM,0);
}

int CTcpServer::Bind(int target_port)
{
    memset(&serveraddr,0,sizeof(struct sockaddr_in));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serveraddr.sin_port=htons(target_port);
    return bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
}

int CTcpServer::Listen(int max_connect)
{
    logger()<<"Socket start listening";
    return listen(sockfd,max_connect);
}

void CTcpServer::Accept()
{
    struct sockaddr_in clientaddr;
    socklen_t len=sizeof(clientaddr);
    clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
}

void CTcpServer::Receive()
{
    QString acc_data;
    QString client_ip;

    while(1)
    {
        memset(recv_content,0,sizeof(recv_content));
        int recv_count=recv(clientfd,recv_content,512,0);
        if(recv_count==0)
        {
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            if (getpeername(clientfd, (struct sockaddr*)&client_addr, &addr_len) == -1) {
                perror("getpeername");
                return;
            }
            client_ip = QString::fromUtf8(inet_ntoa(client_addr.sin_addr));
            logger() << "Client IP: " << client_ip;
            Close();
            break;
        }
        acc_data+=QString::fromUtf8(recv_content);
    }
    logger()<<"Socket received data: "<<acc_data;

    emit ReceivedData(acc_data);
    emit ReceivedDataWithIP(acc_data,client_ip);
}

int CTcpServer::Send(char* message)
{
    return send(clientfd,message,strlen(message),0);
}

void CTcpServer::Close()
{
    close(clientfd);
}


