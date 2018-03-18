#include "ThreadWorker.h"

ThreadWorker::ThreadWorker(int i) : _id(i)
{ 
	printf("Thread%d Start\n", _id);
	_thread = new std::thread(&ThreadWorker::Running, this); 
};

ThreadWorker::~ThreadWorker() 
{ 
	_dead = true;
	_thread->join();
	printf("Thread%d End\n", _id);
}

void ThreadWorker::Running()
{
	while (!_dead)
	{
		if (a != nullptr)
		{
			a->Get_System()->Update(a->Get_JobType(), a->Get_Data());
			a = nullptr;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}

