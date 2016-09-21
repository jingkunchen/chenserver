#ifndef __CHEN_SERVER_H__
#define __CHEN_SERVER_H__

#define PORT 25531
#define MEM_SIZE 1024
#define MAXCONNECTIONS 1024
#define LISTENPORT 25531
#define LISTENADDR "127.0.0.1"


extern "C" void ServerAccept(int iSvrFd, short iEvent, void *arg);

extern "C" void ServerRead(int iCliFd, short iEvent, void *arg);

int SetNonblock(int fd);


class ChenServer
{
public:
	static ChenServer* Instance();

	int Init(int argc, char** argv);

	int Run();

	int GetSock();

	int NewSocket();

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