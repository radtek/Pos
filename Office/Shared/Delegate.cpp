//---------------------------------------------------------------------------
#pragma hdrstop

#include "Delegate.h"
#include <SyncObjs.hpp>
#include <memory>
#include <queue>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Synchronization
{
//---------------------------------------------------------------------------
class DelegateThread : public TThread
{
public:
	DelegateThread() : TThread(false), criticalSection(new TCriticalSection())
	{
	}
	__fastcall ~DelegateThread()
	{
		criticalSection->Enter();
		while(!delegateQueue.empty())
		{
			delete delegateQueue.front();
			delegateQueue.pop();
		}
		criticalSection->Leave();

		Terminate();
		WaitFor();
	}
	void add(Delegate* delegate)
	{
		criticalSection->Enter();
		delegateQueue.push(delegate);
		criticalSection->Leave();
	}
	void __fastcall Execute()
	{
		while(!Terminated)
		{
			try
			{
				if(!delegateQueue.empty())
				{
					criticalSection->Enter();
					currentDelegate = delegateQueue.front();
					delegateQueue.pop();
					criticalSection->Leave();

					try
					{
						Synchronize(executeDelegate);
					}
					__finally
					{
						delete currentDelegate;
					}
				}
				else
				{
					Sleep(100);
				}
			}
			catch(Exception& e)
			{
			}
		}
	}
	void __fastcall executeDelegate()
	{
		(*currentDelegate)();
	}
private:
	const std::auto_ptr<TCriticalSection> criticalSection;
	std::queue<Delegate*> delegateQueue;
	Delegate* currentDelegate;
};
} // namespace
//---------------------------------------------------------------------------
using namespace Synchronization;
//---------------------------------------------------------------------------
SingletonDestroyer::SingletonDestroyer(DelegateQueue* _singleton)
{
	singleton = _singleton;
}
//---------------------------------------------------------------------------
SingletonDestroyer::~SingletonDestroyer()
{
	delete singleton;
}
//---------------------------------------------------------------------------
void SingletonDestroyer::SetSingleton(DelegateQueue* _singleton)
{
	singleton = _singleton;
}
//---------------------------------------------------------------------------
DelegateQueue* DelegateQueue::instance = 0;
SingletonDestroyer DelegateQueue::destroyer;
//---------------------------------------------------------------------------
DelegateQueue::DelegateQueue() : delegateThread(new DelegateThread())
{
}
//---------------------------------------------------------------------------
DelegateQueue::~DelegateQueue()
{
//	terminate();
//	delete delegateThread;
}
//---------------------------------------------------------------------------
/*void DelegateQueue::terminate()
{
	getCriticalSection()->Enter();
	if(instance!=0 && instance->delegateThread != 0)
	{
		instance->delegateThread->Terminate();
		instance->delegateThread->WaitFor();
		delete instance->delegateThread;
		instance->delegateThread = 0;
	}
	getCriticalSection()->Leave();
}*/
//---------------------------------------------------------------------------
void DelegateQueue::add(Delegate* delegate)
{
	getInstance()->delegateThread->add(delegate);
}
//---------------------------------------------------------------------------
DelegateQueue* DelegateQueue::getInstance()
{
	if(instance==0)
	{
		getCriticalSection()->Enter();
		if(instance==0)
		{
			instance = new DelegateQueue();
			destroyer.SetSingleton(instance);
		}
		getCriticalSection()->Leave();
	}
/*	if(instance->delegateThread == 0)
	{
		getCriticalSection()->Enter();
		if(instance->delegateThread == 0)
		{
			instance->delegateThread = new DelegateThread();
		}
		getCriticalSection()->Leave();
	}*/
	return instance;
}
//---------------------------------------------------------------------------
std::auto_ptr<Syncobjs::TCriticalSection> DelegateQueue::criticalSection(0);
//---------------------------------------------------------------------------
Syncobjs::TCriticalSection* DelegateQueue::getCriticalSection()
{
	if(criticalSection.get() == 0)
	{
		criticalSection.reset(new TCriticalSection());
	}
	return criticalSection.get();
}
//---------------------------------------------------------------------------
/*std::auto_ptr<DelegateThread> DelegateQueue::delegateThread(0);
//---------------------------------------------------------------------------
DelegateThread* DelegateQueue::getDelegateThread()
{
	if(delegateThread.get() == 0)
	{
		delegateThread.reset(new DelegateThread());
	}
	return delegateThread.get();
}*/
//---------------------------------------------------------------------------

