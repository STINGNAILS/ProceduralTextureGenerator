#include "stdafx.h"
#include "EnvironmentTaskQueue.h"


EnvironmentTaskQueue::EnvironmentTaskQueue()
{

}


EnvironmentTaskQueue::~EnvironmentTaskQueue()
{

}


void EnvironmentTaskQueue::AddTask(shared_ptr<Environment> environment, int environmentMapIndex)
{
	tasks.push(pair<shared_ptr<Environment>, int>(environment, environmentMapIndex));
}


void EnvironmentTaskQueue::Process()
{
	while(!tasks.empty())
	{
		pair<shared_ptr<Environment>, int> task = tasks.front();

		switch(task.second)
		{
			case 0:
			{
				task.first->SetEnvironmentMap(L"Environment\\Citadella.dds", task.second);
				break;
			}
			case 1:
			{
				task.first->SetEnvironmentMap(L"Environment\\DallasW.dds", task.second);
				break;
			}
			case 2:
			{
				task.first->SetEnvironmentMap(L"Environment\\GoldenGateBridge.dds", task.second);
				break;
			}
			case 3:
			{
				task.first->SetEnvironmentMap(L"Environment\\MarriottMadisonWest.dds", task.second);
				break;
			}
			case 4:
			{
				task.first->SetEnvironmentMap(L"Environment\\Storforsen.dds", task.second);
				break;
			}
			case 5:
			{
				task.first->SetEnvironmentMap(L"Environment\\Storforsen2.dds", task.second);
				break;
			}
			case 6:
			{
				task.first->SetEnvironmentMap(L"Environment\\SwedishRoyalCastle.dds", task.second);
				break;
			}
			case 7:
			{
				task.first->SetEnvironmentMap(L"Environment\\Tenerife.dds", task.second);
				break;
			}
			case 8:
			{
				task.first->SetEnvironmentMap(L"Environment\\UnionSquare.dds", task.second);
				break;
			}
			case 9:
			{
				task.first->SetEnvironmentMap(L"Environment\\Yokohama.dds", task.second);
				break;
			}
			default:
			{
				break;
			}
		}

		tasks.pop();
	}
}
