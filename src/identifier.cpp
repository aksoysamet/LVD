#include "identifier.h"

#include <functional>
#include <queue>
#include <vector>

Identifier::Identifier()
{
	highestID = 0;
}

int Identifier::get()
{
	int id = 0;
	if (!removedIDs.empty())
	{
		id = removedIDs.top();
		removedIDs.pop();
	}
	else
	{
		id = ++highestID;
	}
	return id;
}

void Identifier::remove(int id, std::size_t remaining)
{
	if (remaining > 1)
	{
		removedIDs.push(id);
	}
	else
	{
		reset();
	}
}

void Identifier::reset()
{
	highestID = 0;
	removedIDs = std::priority_queue<int, std::vector<int>, std::greater<int> >();
}