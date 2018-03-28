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
		if ((a = Manager::instance().Give_Job()) != nullptr)
		{
			a->Get_System()->Update(a->Get_JobType(), _flag, a->Get_Data());
			a = nullptr;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(300));
			//std::this_thread::yield();
		}
	}
}

