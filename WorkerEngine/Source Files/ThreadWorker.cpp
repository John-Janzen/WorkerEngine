#include "ThreadWorker.h"

ThreadWorker::ThreadWorker(int i)
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
			a->Get_System()->Update(a->Get_JobType(), a->Get_Data());
			a = nullptr;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

