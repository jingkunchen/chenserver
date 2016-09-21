#include "ChenServer.h"
#include "ChenCommon.h"
#include <stdio.h>
#include <stdlib.h>
#include <event.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/tcp.h>



//static
ChenServer* ChenServer::m_chen_server = NULL;

extern struct event_base* base; 
//extern struct event* ServerEvent;
//extern int iSvrFd;

ChenServer::ChenServer()
{
}

ChenServer* ChenServer::Instance()
{
	if(NULL == m_chen_server)
	{
		m_chen_server = new ChenServer();
	}
	return m_chen_server;
}

int ChenServer::Init(int argc, char** argv)
{
	printf("ChenServer::init\n");

	return RET_SUCCESS;
}

int ChenServer::Run()
{
	printf("ChenServer::run\n");

	return RET_SUCCESS;
}

int SetNonblock(int fd)
{
    int flags;
    
    if ((flags = fcntl(fd, F_GETFL)) == -1) 
    { 
        return RET_FAILURE;
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) 
    {
        return RET_FAILURE;
    }

    return 0;
}


int ChenServer::NewSocket()
{
    //struct event ServerEvent;
    int iSvrFd;

	struct sockaddr_in sSvrAddr;
	iSvrFd = socket(AF_INET, SOCK_STREAM, 0);
    if (iSvrFd == RET_FAILURE) 
    {
        printf("socket(): can not create server socket\n");
        return RET_FAILURE;
    }

    if (SetNonblock(iSvrFd) == RET_FAILURE) 
    {
        return RET_FAILURE;
    }

    memset(&sSvrAddr, 0, sizeof(sSvrAddr));
    sSvrAddr.sin_family = AF_INET;

    sSvrAddr.sin_port = htons(LISTENPORT);

    sSvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(iSvrFd, (struct sockaddr*)&sSvrAddr, sizeof(sSvrAddr)) == RET_FAILURE) {
        close(iSvrFd);
        printf("bind(): can not bind server socket\n");
        return RET_FAILURE;
    }

    if (listen(iSvrFd, MAXCONNECTIONS) == RET_FAILURE) {
        printf("listen(): can not listen server socket\n");
        close(iSvrFd);
        return RET_FAILURE;
    }

    base = event_base_new();
    struct event evListen;
    event_set(&evListen, iSvrFd, EV_READ | EV_PERSIST, ServerAccept, NULL);
    event_base_set(base, &evListen); 
    if (event_add(&evListen, NULL) == RET_FAILURE) {
        printf("event_add(): can not add accept event into libevent\n");
        close(iSvrFd);
        return RET_FAILURE;
    }
    event_base_dispatch(base); 
    return RET_SUCCESS;
}

void ServerAccept(int iSvrFd, short iEvent, void *arg)
{
	int iCliFd; 
	int yes = 1;  
    struct sockaddr_in sCliAddr; 
    socklen_t iSinSize = sizeof(sCliAddr);  
    iCliFd = accept(iSvrFd, (struct sockaddr*)&sCliAddr, &iSinSize); 
    if(iCliFd == RET_FAILURE)
    {
    	printf("accept(): can not accept client connection\n");
    	return;
    }
    if(SetNonblock(iCliFd) == RET_FAILURE)
    {
    	close(iCliFd);
    	return;
    }

    if (setsockopt(iCliFd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) 
    {
        printf("setsockopt(): TCP_NODELAY %s\n", strerror(errno));
        close(iCliFd);
        return;
    }
    struct event *pEvRead = new event;  
    event_set(pEvRead, iCliFd, EV_READ|EV_PERSIST, ServerRead, pEvRead);
    event_base_set(base, pEvRead);
    event_add(pEvRead, NULL);
    printf("Accepted connection from %s\n", inet_ntoa(sCliAddr.sin_addr));

}
void ServerRead(int iCliFd, short iEvent, void *arg)
{
	int iLen;  
    char buf[1500];
    iLen = recv(iCliFd, buf, 1500, 0); 
    if (iLen <= 0) {
    	printf("Client Close\n");
    	struct event *pEvRead = (struct event*)arg;  
        event_del(pEvRead);  
        delete pEvRead;  
        close(iCliFd);  
        return;  
    }
    buf[iLen] = 0;  
    printf("Client Info:%s\n",buf);  
}



void ChenServer::Destroy()
{
	exit(0);
}

ChenServer::~ChenServer()
{
}
