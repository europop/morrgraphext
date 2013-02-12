#include "Threading.h"
#include <assert.h>

//-----------------------------------------------------------------------------
// SimpleMutex class
//-----------------------------------------------------------------------------

SimpleMutex::SimpleMutex() {
	InitializeCriticalSection(&m_crit_sec);
}

//-----------------------------------------------------------------------------

SimpleMutex::~SimpleMutex() {
	DeleteCriticalSection(&m_crit_sec);
}

//-----------------------------------------------------------------------------
// ThreadFlag class
//-----------------------------------------------------------------------------

ThreadFlag::ThreadFlag() : m_complete(false) {}

//-----------------------------------------------------------------------------

ThreadFlag::~ThreadFlag() {}

//-----------------------------------------------------------------------------

void ThreadFlag::Wait() {
	//Loop until the task is complete and then delete it
	while(!m_complete) {};
}

//-----------------------------------------------------------------------------

void ThreadFlag::Wait(DWORD dwSleepTime) {
	//Loop until the task is complete and then delete it
	while(!m_complete) { Sleep(dwSleepTime); };
}

//-----------------------------------------------------------------------------

bool ThreadFlag::IsComplete() {
	//m_mutex.Lock();
	return m_complete;
	//m_mutex.Unlock();
}
//-----------------------------------------------------------------------------

void ThreadFlag::_MarkComplete() {
	//m_mutex.Lock();
	m_complete = true;
	//m_mutex.Unlock();
}

//-----------------------------------------------------------------------------
// ThreadTask class
//-----------------------------------------------------------------------------

ThreadTask::ThreadTask(ThreadFlag* flag) : m_flag(flag) {}

//-----------------------------------------------------------------------------

ThreadTask::~ThreadTask() {
	//Mark the flag complete so that anyone who is waiting on this task knows
	//it is done
	m_flag->_MarkComplete();
}

//-----------------------------------------------------------------------------

void SimpleMutex::Unlock() {
	LeaveCriticalSection(&m_crit_sec);
}

//-----------------------------------------------------------------------------

void SimpleMutex::Lock() {
	EnterCriticalSection(&m_crit_sec);
}

//-----------------------------------------------------------------------------
// WorkerThread class
//-----------------------------------------------------------------------------

WorkerThread::WorkerThread(TaskScheduler* scheduler, bool createThread) :
	m_stop(false),
	m_scheduler(scheduler),
	m_idle(false)
{
	// Create the idle semaphore with initial count of 0 and max of 1.
	// This means that the first call to WaitOnSingleObject will cause a thread to
	// idle until someone else increments the count with a call to ReleaseSemaphore.
	m_idle_semaphore = CreateSemaphore( NULL, 0, 1, NULL );

	//Start the thread
	DWORD thread_id;
	m_thread_handle = createThread ? CreateThread(NULL, STACKSIZE, _ThreadProc, this, 0, &thread_id) : NULL;
}

//-----------------------------------------------------------------------------

WorkerThread::~WorkerThread() {
	//Wait for the thread to finish
	m_stop = true;

	//If the thread is idle, release the semaphore to wake it
	if ( m_idle == true ) {
		ReleaseSemaphore( m_idle_semaphore, 1, NULL );
	}

	if (m_thread_handle) {
		WaitForSingleObject( m_thread_handle, INFINITE );
		CloseHandle(m_thread_handle);
	}
	CloseHandle(m_idle_semaphore);
	m_thread_handle = 0;
}

//-----------------------------------------------------------------------------

void WorkerThread::DoTasks() {
	
	ThreadTask* task = 0;

	do
	{
		//Process any tasks in our queue
		while (task = PopTask())
		{
			task->Run();
			delete task;
		}

		//We ran out of tasks, so try to see if we can steal one
		task = StealTask();
		if (task) {
			task->Run();
			delete task;
		}

	} while (task);	
}

//-----------------------------------------------------------------------------

void WorkerThread::PushTask(ThreadTask* task) {
	m_mutex.Lock();
	{
		m_tasks.push_back(task);

		//If the thread is idle, release the semaphore to wake it
		if ( m_idle == true ) {
			ReleaseSemaphore( m_idle_semaphore, 1, NULL );
			m_idle = false;
		}
	}
	m_mutex.Unlock();
}

//-----------------------------------------------------------------------------

ThreadTask* WorkerThread::PopTask() {
	ThreadTask* task = 0;

	m_mutex.Lock();
	{
		if ( m_tasks.size() > 0 ) {
			task = m_tasks.front();
			m_tasks.pop_front();
		}
	}
	m_mutex.Unlock();

	return task;
}

//-----------------------------------------------------------------------------

void WorkerThread::Idle() {

	m_mutex.Lock();
	{
		assert( m_idle == false );

		if ( m_tasks.size() == 0 ) {
			m_idle = true;
		}
	}
	m_mutex.Unlock();

	// Wait for the semaphore to be decremented by PushTask
	if (m_idle) {
		WaitForSingleObject( m_idle_semaphore, INFINITE );
	}
}

//-----------------------------------------------------------------------------

ThreadTask* WorkerThread::StealTask() {

	//Do nothing if there are no other threads
	if (m_scheduler->m_num_workers < size_t(&m_scheduler->m_workers[m_scheduler->m_num_workers] == this ? 1 : 2) ) {
		return 0;
	}

	bool tasks_stolen = false;

	//Loop through other tasks looking for tasks to steal
	size_t num_workers = m_scheduler->m_num_workers;
	for ( size_t i = 0; i < num_workers; ++i ) {
		if ( ThreadTask* task = m_scheduler->m_workers[i].PopTask() ) {
			//we got a task, so don't query any other worker threads.
			return task;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------

size_t WorkerThread::GetTaskCount() {
	size_t count = 0;
	
	m_mutex.Lock();
	{
		count = m_tasks.size();
	}
	m_mutex.Unlock();

	return count;
}

//-----------------------------------------------------------------------------

DWORD WINAPI WorkerThread::_ThreadProc(void* p) {
	WorkerThread* thread = (WorkerThread*)p;

	while(true) 	{			
		// Check if we're shutting down
		if ( thread->m_stop ) {
			break;
		}
		
		// Work on tasks
		thread->DoTasks();

		// Tasks are all done, so go idle
		thread->Idle();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// TaskScheduler class
//-----------------------------------------------------------------------------

TaskScheduler::TaskScheduler() {

//You can change this to zero to run in single-threaded mode if you want
#if 1
	//Find out how many logical processors this system has
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	m_num_workers = si.dwNumberOfProcessors - 1;

	////Since Morrowind already has a worker thread, it might be necessary to drop one
	////core to try to decrease the chance that any of our workers end up on the same core.
	////Seems to hurt performance by a tiny bit instead of helping now that I'm using
	////semaphores to idle non-working threads, so maybe it's not necessary.
	//if (m_num_workers > 2 ) {
	//	m_num_workers -= 1;
	//}

	if (m_num_workers > 0) {
		m_workers = (WorkerThread*)malloc( sizeof(WorkerThread)*(m_num_workers + 1) );

		//Construct workers with placment new so that we can pass something
		//into their constructor
		for (size_t i = 0; i < m_num_workers; ++i) {
			::new(&m_workers[i]) WorkerThread(this, true);
		}
		::new(&m_workers[m_num_workers]) WorkerThread(this, false);
	}
#else
	m_num_workers = 0;
#endif

	m_next_thread_to_schedule = 0;
}

//-----------------------------------------------------------------------------

TaskScheduler::~TaskScheduler() {
	//Destruct workers.  This will also stop their threads.
	if (m_num_workers > 0) {
		for (size_t i = 0; i <= m_num_workers; ++i) {
			m_workers[i].~WorkerThread();
		}
	}

	free(m_workers);
}

//-----------------------------------------------------------------------------

void TaskScheduler::ScheduleTask(ThreadTask* task) {
	//If we're on a single core machine, we may not have created any threads
	if ( m_num_workers == 0 ) {
		//execute task immediately and return
		task->Run();
		delete task;
		return;
	}

	//Schedule the task on the next thread in the array
	m_workers[m_next_thread_to_schedule].PushTask(task);

	//Determine which thread will be the next to get a task
	++m_next_thread_to_schedule;
	if (m_next_thread_to_schedule >= m_num_workers) {
		m_next_thread_to_schedule = 0;
	}

	/*

	//Find the worker with the smallest number of tasks and add this task to it
	WorkerThread* lowest_thread = 0;
	size_t lowest_count = 0xFFFFFFFF;
	for ( size_t i = 0; i < m_num_workers; ++i ) {
		size_t count = m_workers[i].GetTaskCount();
		if ( count < lowest_count ) {
			lowest_thread = &m_workers[i];
			lowest_count = count;
		}
	}

	lowest_thread->PushTask(task);
	*/
}

//-----------------------------------------------------------------------------

void TaskScheduler::DoTasksInCurrentThread() {
	if (m_num_workers > 0) {
		m_workers[m_num_workers].DoTasks();
	}
}

//-----------------------------------------------------------------------------
