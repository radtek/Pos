//---------------------------------------------------------------------------
#ifndef BackgroundWorkerH
#define BackgroundWorkerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <set>
//---------------------------------------------------------------------------
namespace Syncobjs { class TCriticalSection; }
//---------------------------------------------------------------------------
namespace Utils
{
//---------------------------------------------------------------------------
class IBackgroundWorker
{
public:
	virtual void Run() = 0;
	virtual void Stop() = 0;
};
//---------------------------------------------------------------------------
class BackgroundWorkerFactory
{
public:

	BackgroundWorkerFactory();
	virtual ~BackgroundWorkerFactory();

	template <typename TResult>
	void RunWorker(TResult __fastcall (__closure * action)(), void __fastcall (__closure * completed)(IBackgroundWorker* worker, TResult result))
	{
		IBackgroundWorker* backgroundWorker = new BackgroundWorker<TResult, int>(
			action,
			completed,
			&this->Unregister,
			0);

		this->RegisterWorker(backgroundWorker);

		backgroundWorker->Run();
	}

	template <typename TResult, typename TState>
	void RunWorker(TResult __fastcall (__closure * action)(TState), void __fastcall (__closure * completed)(IBackgroundWorker* worker, TResult result), TState state)
	{
		IBackgroundWorker* backgroundWorker = new BackgroundWorker<TResult, TState>(
			action,
			completed,
			&this->Unregister,
			state);

		this->RegisterWorker(backgroundWorker);

		backgroundWorker->Run();
	}

	private:
	Syncobjs::TCriticalSection* const sync;
	std::set<Utils::IBackgroundWorker*> backgroundThreads;
	void RegisterWorker(Utils::IBackgroundWorker* worker);
	void UnregisterWorker(IBackgroundWorker* worker);
	void TerminateWorkers();

	void __fastcall Unregister(IBackgroundWorker* backgroundWorker)
	{
		this->UnregisterWorker(backgroundWorker);
	}
};
//---------------------------------------------------------------------------
template <typename TResult, typename TState>
class BackgroundWorker : protected TThread, public IBackgroundWorker
{
public:

	typedef TResult __fastcall (__closure *DoWorkWithStateEvent)(TState state);
	typedef TResult __fastcall (__closure *DoWorkWithoutStateEvent)();
	typedef void __fastcall (__closure *WorkCompletedEvent)(IBackgroundWorker* worker, TResult result);
	typedef void __fastcall (__closure *WorkerUnregisterEvent)(IBackgroundWorker* worker);

	BackgroundWorker(DoWorkWithoutStateEvent doWork, WorkCompletedEvent completed, WorkerUnregisterEvent unregister, TState state)
		: TThread(true), state(state)
	{
		this->Priority = tpLower;
		this->FreeOnTerminate = true;

		this->doWorkWithoutState = doWork;
		this->doWorkWithState = NULL;

		this->completed = completed;
		this->unregister = unregister;
	}

	BackgroundWorker(DoWorkWithStateEvent doWork, WorkCompletedEvent completed, WorkerUnregisterEvent unregister, TState state)
		: TThread(true), state(state)
	{
		this->Priority = tpLower;
		this->FreeOnTerminate = true;

		this->doWorkWithoutState = NULL;
		this->doWorkWithState = doWork;

		this->completed = completed;
		this->unregister = unregister;
	}

	virtual __fastcall ~BackgroundWorker() { }

	virtual void Run()
	{
		this->Resume();
	}

	virtual void Stop()
	{
		this->Terminate();
	}

protected:

	void __fastcall Execute()
	{
		try
		{
			if (this->Terminated == false)
				this->DoWork();
		}
		__finally
		{
			this->unregister(this);
		}

		if (this->Terminated == false)
			Synchronize(&this->Completed);
	}

	__fastcall virtual void DoWork()
	{
		if (this->doWorkWithState != NULL)
		{
			this->result = this->doWorkWithState(this->state);
		}
		else if (this->doWorkWithoutState != NULL)
		{
			this->result = this->doWorkWithoutState();
		}
	}

	__fastcall virtual void Completed()
	{
		this->completed(this, this->result);
	}

private:

	DoWorkWithStateEvent doWorkWithState;
	DoWorkWithoutStateEvent doWorkWithoutState;

	WorkCompletedEvent completed;
	WorkerUnregisterEvent unregister;

	TState state;
	TResult result;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
