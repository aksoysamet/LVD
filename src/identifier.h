#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <functional>
#include <queue>
#include <vector>

class Identifier
{
public:
	Identifier();

	int get();
	void remove(int id, std::size_t remaining);
	void reset();
private:
	int highestID;

	std::priority_queue<int, std::vector<int>, std::greater<int> > removedIDs;
};

#endif