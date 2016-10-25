#include <iostream>
#include <unistd.h>

using namespace std;

ChenMutex g_Lock;

void * StartThread(void *pParam)
{
	char *pMsg = (char *)pParam;
	if (!pMsg)
	{
		return (void *)1;
	}
	
	ChenLock lock(g_Lock);

	for( int i = 0; i < 5; i++ )
	{
		cout << pMsg << endl;
		sleep( 1 );
	}

	return (void *)0;
}

int main(int argc, char* argv[])
{
	pthread_t thread1,thread2;
	pthread_attr_t attr1,attr2;

	char *pMsg1 = "First print thread.";
	char *pMsg2 = "Second print thread.";

	pthread_attr_init(&attr1);
	pthread_attr_setdetachstate(&attr1,PTHREAD_CREATE_JOINABLE);
	if (pthread_create(&thread1,&attr1, StartThread,pMsg1) == -1)
	{
		cout<<"Thread 1: create failed"<<endl;
	}
	pthread_attr_init(&attr2);
	pthread_attr_setdetachstate(&attr2,PTHREAD_CREATE_JOINABLE);
	if (pthread_create(&thread2,&attr2, StartThread,pMsg2) == -1)
	{
		cout<<"Thread 2: create failed"<<endl;
	}

	void *result;
	pthread_join(thread1,&result);
	pthread_join(thread2,&result);

	pthread_attr_destroy(&attr1);
	pthread_attr_destroy(&attr2);

	int iWait;
	cin>>iWait;

	return 0;
}

