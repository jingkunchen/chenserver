#ifndef __CHEN_SERVER_H__
#define __CHEN_SERVER_H__

#define MEM_SIZE 1024
#define MAXCONNECTIONS 1024
#define LISTENPORT 25531
#define LISTENADDR "127.0.0.1"


extern "C" void ServerRead(int iCliFd, short iEvent, void *arg);

extern "C" void pthread_pub(void *(*func)(void *), void *arg);

extern "C" void *libevent_pub(void *arg);

extern "C" void ServerAccept(int iSvrFd, short iEvent, void *arg);


class ChenServer
{
public:
	static ChenServer* Instance();

	int Init(int argc, char** argv);

	int Run();

	int NewSocket();
	//use fcntl set
	int SetNonBlock(int fd);

	int SetBlock(int fd);

	//use ioctl set
	int  SetNonBlocking(int fd);

	int  SetBlocking(int fd);

	void thread_pool_init(int number);
	
	static void Destroy();

	virtual ~ChenServer();

public:
	static ChenServer *m_chen_server;

private:
	ChenServer();

	ChenServer(const ChenServer&);

	void operator= (const ChenServer&);
};
#endif
