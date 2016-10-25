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
#include <sys/ioctl.h>




//static
ChenServer* ChenServer::m_chen_server = NULL;
struct event_base* base;  
pthread_mutex_t i_lock;
pthread_cond_t i_cond;
int thread_count;
EVENT_HANDLER *handler;
ConnQueue Conn_Queue[THREADNUMBER];


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
	
	this->thread_pool_init(THREADNUMBER);

	return RET_SUCCESS;
}

int ChenServer::Run()
{
	int ret;
	
	printf("ChenServer::run\n");

	ret = this->NewSocket();
	
	if(ret == RET_FAILURE)
    {
    	return RET_FAILURE;
    }

	return RET_SUCCESS;
}

<<<<<<< HEAD
int ChenServer::SetNonBlock(int fd)
=======
int ChenServer::SetNonblock(int fd)
>>>>>>> cf378fbf8fb31507bef21f0b403c35c6d53955dd
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

int ChenServer::SetBlock(int fd)
{
    int flags;
    
    if ((flags = fcntl(fd, F_GETFL)) == -1) 
    { 
        return RET_FAILURE;
    }

    if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) == -1) 
    {
        return RET_FAILURE;
    }

    return RET_SUCCESS;
}

int ChenServer::SetNonBlocking(int fd)
{
	int  nb;
	
    nb = 1;

    return ioctl(fd, FIONBIO, &nb);
}

int ChenServer::SetBlocking(int fd)
{
	int  nb;
	
    nb = 0;

    return ioctl(fd, FIONBIO, &nb);
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

    pthread_mutex_lock(&i_lock);
	printf("worker_libevent\n");
	thread_count++;
    pthread_cond_signal(&i_cond);
    pthread_mutex_unlock(&i_lock);

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

void ChenServer::thread_pool_init(int number)
{
	printf("thread_pool_init\n");
	int i;
	pthread_mutex_init(&i_lock, NULL);
	pthread_cond_init(&i_cond, NULL);
	thread_count =0;

	handler = (EVENT_HANDLER *)calloc(number,sizeof(EVENT_HANDLER));
	for(i=0; i< number; i++)
	{
		int fds[2];
		if(pipe(fds))
		{
			printf("pipe error\n");
			exit(-1);
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
	
	pthread_mutex_lock(&i_lock);
	   while(thread_count < number)
	   {
		   pthread_cond_wait(&i_cond, &i_lock);
	   }

    pthread_mutex_unlock(&i_lock);

}



int ChenServer::NewSocket()
{
    int iSvrFd;

	struct sockaddr_in sSvrAddr;
	iSvrFd = socket(AF_INET, SOCK_STREAM, 0);

    if (iSvrFd == RET_FAILURE) 
    {
        printf("socket(): can not create server socket\n");
        return RET_FAILURE;
    }

<<<<<<< HEAD
    if (this->SetNonBlock(iSvrFd) == RET_FAILURE) 
=======
    if (this->SetNonblock(iSvrFd) == RET_FAILURE) 
>>>>>>> cf378fbf8fb31507bef21f0b403c35c6d53955dd
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
    event_set(&evListen, iSvrFd, EV_READ | EV_PERSIST, ServerAccept, this);
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
	ChenServer* detailJob = static_cast<ChenServer*>(arg);
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
<<<<<<< HEAD
    if(detailJob->SetNonBlock(iCliFd) == RET_FAILURE)
=======
    if(detailJob->SetNonblock(iCliFd) == RET_FAILURE)
>>>>>>> cf378fbf8fb31507bef21f0b403c35c6d53955dd
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
	if(NULL != m_chen_server)
	{
		delete m_chen_server;
		m_chen_server = NULL;
	}

	exit(0);
}

ChenServer::~ChenServer()
{
	
}
