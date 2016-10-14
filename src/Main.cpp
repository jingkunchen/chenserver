#include "ChenServer.h"
#include "ChenCommon.h"
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdio.h>
#include <event.h>
#include <arpa/inet.h>
#include <string.h>

struct event_base* base;  
pthread_mutex_t init_lock;
pthread_cond_t init_cond;
int thread_count;
EVENT_HANDLER *handler;
ConnQueue Conn_Queue[THREADNUMBER];

int main(int argc, char ** argv)
{
	int ret =0;  
	struct sockaddr_in sSvrAddr;
	memset(&sSvrAddr, 0, sizeof(sSvrAddr));

	ChenServer* chen_server = NULL;

	chen_server = ChenServer::Instance();
    
    ret = chen_server->Init(argc,argv);
    if(ret == RET_SUCCESS)
    {
    	printf("chen_server->init success\n");
    }

    ret = chen_server->Run();
    if(ret == RET_SUCCESS)
    {
    	printf("chen_server->run success\n");
    }
    event_init();

	thread_pool_init(THREADNUMBER);

    ret =chen_server->NewSocket();
    if(ret == RET_FAILURE)
    {
    	return RET_FAILURE;
    }
    event_dispatch();

    return 0;

}