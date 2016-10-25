#include "ChenServer.h"
#include "ChenCommon.h"
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdio.h>
#include <event.h>
#include <arpa/inet.h>
#include <string.h>
#include "ChenLock.h"

<<<<<<< HEAD

using namespace std;

ChenMutex g_Lock;

=======
>>>>>>> cf378fbf8fb31507bef21f0b403c35c6d53955dd
int main(int argc, char ** argv)
{
	int ret =0;  

	ChenServer* chen_server = ChenServer::Instance();
    
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

	ChenServer::Destroy();
    return 0;

}