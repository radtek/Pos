//---------------------------------------------------------------------------
#ifndef DelegateH
#define DelegateH
//---------------------------------------------------------------------------
#include <memory>
//---------------------------------------------------------------------------
namespace Syncobjs
{
	class TCriticalSection;
}
//---------------------------------------------------------------------------
namespace Synchronization
{
//---------------------------------------------------------------------------
class Delegate
{
public:
	virtual void operator ()() = 0;
	virtual ~Delegate()
	{
	}
};
//---------------------------------------------------------------------------
class DelegateQueue;
//---------------------------------------------------------------------------
class SingletonDestroyer
{
public:
	SingletonDestroyer(DelegateQueue* _singleton = 0);
	~SingletonDestroyer();

	void SetSingleton(DelegateQueue* _singleton);
private:
	DelegateQueue* singleton;
};
//---------------------------------------------------------------------------
class DelegateThread;
//---------------------------------------------------------------------------
class DelegateQueue
{
public:
	static void add(Delegate* delegate);
//	static void terminate();
private:
	friend class SingletonDestroyer;
	DelegateQueue();
	~DelegateQueue();

	static SingletonDestroyer destroyer;

	static DelegateQueue* instance;
	static DelegateQueue* getInstance();

	static Syncobjs::TCriticalSection* getCriticalSection();
	static std::auto_ptr<Syncobjs::TCriticalSection> criticalSection;

	std::auto_ptr<DelegateThread> delegateThread;
//	static DelegateThread* getDelegateThread();
//	static std::auto_ptr<DelegateThread> delegateThread;
//	class DelegateThread* delegateThread;
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
