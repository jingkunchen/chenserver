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
#include <pthread.h>



//static
ChenServer* ChenServer::m_chen_server = NULL;
extern int thread_count;
extern struct event_base* base; 
extern pthread_mutex_t init_lock;
extern pthread_cond_t init_cond;
extern EVENT_HANDLER *handler;
extern ConnQueue Conn_Queue[THREADNUMBER];


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

void handler_control(int iSvrFd, short iEvent, void *arg)
{
    EVENT_HANDLER *handler = (EVENT_HANDLER *)arg;
    pthread_mutex_lock(&Conn_Queue[handler->index].lock);
	
    if(Conn_Queue[handler->index].fdQ.empty())
	{
		pthread_mutex_unlock(&Conn_Queue[handler->index].lock);
		return ;
    }
	
	int iCliFd = Conn_Queue[handler->index].fdQ.front();
	printf("iCliFd:%d",iCliFd);
	Conn_Queue[handler->index].fdQ.pop();
	pthread_mutex_unlock(&Conn_Queue[handler->index].lock);
	printf("iSvrFd:%d\n",iSvrFd);
	printf("iEvent:%d\n",iEvent);
	printf("arg:%p\n",arg);

	struct event *pEvRead = new event;  
	event_set(pEvRead, iCliFd, EV_READ|EV_PERSIST, ServerRead, pEvRead);
	event_base_set(base, pEvRead);
	event_add(pEvRead, NULL);

}

void *libevent_pub(void *arg)
{
    EVENT_HANDLER	*me	= (EVENT_HANDLER *)arg;

    pthread_mutex_lock(&init_lock);
	printf("worker_libevent\n");
	thread_count++;
    pthread_cond_signal(&init_cond);
    pthread_mutex_unlock(&init_lock);

    event_base_loop(me->base, 0);
    return NULL;
}

void pthread_pub(void *(*func)(void *), void *arg)
{
    pthread_t		thread;
    pthread_attr_t	attr;
    int				ret;

    pthread_attr_init(&attr);
    if ((ret = pthread_create(&thread, &attr, func,	arg)) != 0)
	{
		exit(1);
    }
}

void thread_pool_init(int number)
{
	printf("thread_pool_init\n");
	int i;
	pthread_mutex_init(&init_lock, NULL);
	pthread_cond_init(&init_cond, NULL);
	thread_count =0;

	handler = (EVENT_HANDLER *)calloc(number,sizeof(EVENT_HANDLER));
	for(i=0; i< number; i++)
	{
		int fds[2];
		if(pipe(fds))
		{
			printf("pipe error\n");
		}
		
		pthread_mutex_init(&Conn_Queue[i].lock, NULL);
		pthread_cond_init(&Conn_Queue[i].cond, NULL);
		handler[i].index = i;
		handler[i].send_notify_fd = fds[1];
		handler[i].receive_notify_fd = fds[0];
		handler[i].base = event_base_new();
		printf("handler[%d] address:%p\n",i,&handler[i]);
		event_set(&handler[i].notify_event,handler[i].receive_notify_fd, EV_READ|EV_PERSIST, handler_control, &handler[i]);
		event_base_set(handler[i].base, &handler[i].notify_event);
		event_add(&handler[i].notify_event, 0);

	}
	
	for	(i = 0;	i <	number; i++)
	{
		pthread_pub(libevent_pub, &handler[i]);
    }
	
	if(thread_count < number)
	{
		sleep(1);
	}

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

    if (bind(iSvrFd, (struct sockaddr*)&sSvrAddr, sizeof(sSvrAddr)) == RET_FAILURE) 
	{
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
    if (event_add(&evListen, NULL) == RET_FAILURE)
	{
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

	printf("Accepted connection from %s\n", inet_ntoa(sCliAddr.sin_addr));
	int idx = rand() % THREADNUMBER;
    printf("ServerAccept idx:%d\n",idx);
	
	pthread_mutex_lock(&Conn_Queue[idx].lock);
	printf("ServerAccept Queue start\n");
	Conn_Queue[idx].fdQ.push(iCliFd);
	
	pthread_cond_signal(&Conn_Queue[idx].cond);
	pthread_mutex_unlock(&Conn_Queue[idx].lock);
	printf("ServerAccept Queue success\n");
	if (write(handler[idx].send_notify_fd, "", 1) !=1)
	{
		printf("handler_event->send_notify_fd:%d\n",handler[idx].send_notify_fd);
		perror("Writing to thread notify pipe\n");
	}

}
void ServerRead(int iCliFd, short iEvent, void *arg)
{
	int iLen;  
    char buf[1500];
    iLen = recv(iCliFd, buf, 1500, 0); 
    if (iLen <= 0) 
	{
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
