#ifndef _MULTITHREADING_H_
#define _MULTITHREADING_H_

//Multi-threaded task queue system based to some extent on this article:
//http://isdlibrary.intel-dispatch.com/vc/2755/Doityourself_123109_final.pdf
//The main difference is that this system just takes whole tasks and puts them on
//one thread or another which processes them until they're done or stolen by another
//thread.  It has no support for spreading tasks to multiple threads or one task
//spawning another.

#include <windows.h>
#include <list>
#include <vector>
using namespace std;

//-----------------------------------------------------------------------------

/// A wrapper around the Windows critical section API
class SimpleMutex {
public:
	SimpleMutex();
	~SimpleMutex();
	void Lock();
	void Unlock();
private:
	CRITICAL_SECTION m_crit_sec;
};

//-----------------------------------------------------------------------------

//Used to check if tasks are finished
class ThreadFlag {
public:
	ThreadFlag();
	~ThreadFlag();
	void Wait();
	void Wait(DWORD dwSleepTime);
	bool IsComplete();
protected:
	friend class ThreadTask;
	//SimpleMutex m_mutex;
	void _MarkComplete();
	volatile bool m_complete;
};

//-----------------------------------------------------------------------------

/// Represents a non-blocking tasks that will be farmed out to a worker thread
/// Derive from it and implement the Run function to create tasks.
class ThreadTask {
public:
	ThreadTask(ThreadFlag* flag);
	virtual ~ThreadTask();
	virtual void Run() = 0;
protected:
	ThreadFlag* m_flag;
};

//-----------------------------------------------------------------------------

class TaskScheduler;

/// Represents a worker thread that will execute any tasks that it is given
class WorkerThread {
public:
	WorkerThread(TaskScheduler* scheduler, bool createThread);
	~WorkerThread();
	void DoTasks();
	void PushTask(ThreadTask* task);
	ThreadTask* PopTask();
	size_t GetTaskCount();
protected:
	static const SIZE_T STACKSIZE = 65536;

	void Idle();
	ThreadTask* StealTask();
	static DWORD WINAPI _ThreadProc(void* p);

	SimpleMutex m_mutex;
	list<ThreadTask*> m_tasks;
	TaskScheduler* m_scheduler;
	HANDLE m_thread_handle;
	HANDLE m_idle_semaphore;
	volatile bool m_stop;
	volatile bool m_idle;
};

//-----------------------------------------------------------------------------

/// Manages worker threads and hands out tasks to them.

class TaskScheduler {
public:
	TaskScheduler();
	~TaskScheduler();
	void ScheduleTask(ThreadTask* task);
	void DoTasksInCurrentThread();
protected:
	friend class WorkerThread;
	WorkerThread* m_workers;
	size_t m_num_workers;
	size_t m_next_thread_to_schedule;
	bool m_workers_idle;
};

#endif

//-----------------------------------------------------------------------------
