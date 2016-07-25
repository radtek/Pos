//---------------------------------------------------------------------------
#include <SyncObjs.hpp>
#include "BackgroundWorker.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace Utils
{
//---------------------------------------------------------------------------
BackgroundWorkerFactory::BackgroundWorkerFactory() : sync(new TCriticalSection())
{
}
//---------------------------------------------------------------------------
BackgroundWorkerFactory::~BackgroundWorkerFactory()
{
	this->TerminateWorkers();
	delete this->sync;
}
//---------------------------------------------------------------------------
void BackgroundWorkerFactory::RegisterWorker(IBackgroundWorker* worker)
{
	this->sync->Acquire(); // lock out other threads
	try
	{
		this->backgroundThreads.insert(worker);
	}
	__finally
	{
		this->sync->Release();
	}
}
//---------------------------------------------------------------------------
void BackgroundWorkerFactory::UnregisterWorker(IBackgroundWorker* worker)
{
	this->sync->Acquire(); // lock out other threads
	try
	{
		this->backgroundThreads.erase(worker);
	}
	__finally
	{
		this->sync->Release();
	}
}
//---------------------------------------------------------------------------
void BackgroundWorkerFactory::TerminateWorkers()
{
	this->sync->Acquire(); // lock out other threads
	try
	{
		std::set<IBackgroundWorker*>::iterator worker = this->backgroundThreads.begin();
		while (worker != this->backgroundThreads.end())
		{
			IBackgroundWorker* backgroundWorker = *worker;
			backgroundWorker->Stop();
			worker++;
		}

		this->backgroundThreads.erase(this->backgroundThreads.begin(), this->backgroundThreads.end());
	}
	__finally
	{
		this->sync->Release();
	}
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

