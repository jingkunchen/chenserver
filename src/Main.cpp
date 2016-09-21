#include "ChenServer.h"
#include "ChenCommon.h"
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdio.h>
#include <event.h>
#include <arpa/inet.h>
#include <string.h>
#include <event2/event.h>

struct event_base* base;  
//struct event* ServerEvent;
//int iSvrFd;


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

    ret =chen_server->NewSocket();
    if(ret == RET_FAILURE)
    {
    	return RET_FAILURE;
    }
    event_dispatch();

/*
    sSvrAddr.sin_family = AF_INET;    
    sSvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");      
    sSvrAddr.sin_port = htons(LISTENPORT);

    iSvrFd = socket(AF_INET, SOCK_STREAM, 0);    
    bind(iSvrFd, (struct sockaddr*)&sSvrAddr, sizeof(sSvrAddr));    
    listen(iSvrFd, 10); 

    base = event_base_new();
    struct event evListen;

    event_set(&evListen, iSvrFd, EV_READ|EV_PERSIST, ServerAccept, NULL);  

    event_base_set(base, &evListen); 
    event_add(&evListen, NULL);  
    event_base_dispatch(base);  */
    return 0;

}