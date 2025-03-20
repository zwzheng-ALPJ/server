#include "tcp_server.h"

CTcpServer::CTcpServer()
{
    sockfd=socket(AF_INET,SOCK_STREAM,0);

    // counter for stress test
    // packet_count = 0;
    // stress_test_timer = new QTimer(this);
    // connect(stress_test_timer, &QTimer::timeout, this, [this]() {
    //     logger() << "Connections made in the last second:" << packet_count;
    //     packet_count = 0;
    // });
    // stress_test_timer->start(1000);
}

int CTcpServer::Bind(int target_port)
{
    last_port=target_port;
    memset(&serveraddr,0,sizeof(struct sockaddr_in));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serveraddr.sin_port=htons(target_port);
    is_socket_alive = (bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==0)?true:false;
    return is_socket_alive;
}

int CTcpServer::Listen(int max_connect)
{
    last_max_connect=max_connect;
    if(true)
    {
        logger()<<"Socket start listening";
        return listen(sockfd,max_connect);
    }
    return -1;
}

void CTcpServer::Accept()
{
    if(is_socket_alive)
    {
        struct sockaddr_in clientaddr;
        socklen_t len=sizeof(clientaddr);
        clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
    }
}

void CTcpServer::Receive()
{
    if(is_socket_alive)
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
                if (getpeername(clientfd, (struct sockaddr*)&client_addr, &addr_len) == -1)
                {
                    perror("getpeername");
                    return;
                }
                client_ip = QString::fromUtf8(inet_ntoa(client_addr.sin_addr));
                //logger() << "Client IP: " << client_ip;
                Close();
                break;
            }
            acc_data+=QString::fromUtf8(recv_content);
        }
        //logger()<<"Socket received data: "<<acc_data;

        // packet_count++;
        emit ReceivedData(acc_data);
        emit ReceivedDataWithIP(acc_data,client_ip);
    }
}

int CTcpServer::Send(char* message)
{
    if(is_socket_alive)
    {
        return send(clientfd,message,strlen(message),0);
    }
    return -1;
}

void CTcpServer::Close()
{
    if(is_socket_alive)
    {
        close(clientfd);
    }
}

void CTcpServer::Reset()
{
    is_socket_alive=false;
    close(sockfd);
    Bind(last_port);
    Listen(last_max_connect);
    is_socket_alive=true;
}

