#include "ThreadWorker.h"
#include "ThreadPool.h"
#include "System.h"

ThreadWorker::ThreadWorker(int i, ThreadPool * tp) : _parent{tp}
{ 
	sprintf_s(_name, "Thread%d", i);
	printf("%s Start\n", _name);
	_thread = new std::thread(&ThreadWorker::Running, this); 
};

ThreadWorker::~ThreadWorker() 
{ 
	_dead = true;
	_thread->join();
	printf("%s End\n", _name);
}

void ThreadWorker::Running()
{
	while (!_dead)
	{
		if (a != nullptr)
		{
			a->Get_System()->Update(a->Get_JobType(), _flag, a->Get_Data());
			a = nullptr;
			_parent->signalDone(this);
			/*if (_parent->checkBusy())
				printf("SOMETHING'S FUCKY");*/
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(300));
			//std::this_thread::yield();
		}
	}
}

