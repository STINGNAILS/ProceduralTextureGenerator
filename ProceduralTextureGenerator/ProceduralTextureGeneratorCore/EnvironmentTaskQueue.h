#pragma once

#include "Environment.h"


class EnvironmentTaskQueue
{
	private:

	queue<pair<shared_ptr<Environment>, int>> tasks;

	public:

	EnvironmentTaskQueue();
	~EnvironmentTaskQueue();

	void AddTask(shared_ptr<Environment> environment, int environmentMapIndex);

	void Process();
};

