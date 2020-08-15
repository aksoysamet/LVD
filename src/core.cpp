#include "core.h"
typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;
CLevenDis::CLevenDis()
{
}

CLevenDis::~CLevenDis()
{
	for (auto i = SystemList.begin(); i != SystemList.end(); i++)
	{
		delete i->second;
	}
	SystemList.clear();
}

int CLevenDis::AddLevenSystem(bool search_cmds)
{
	int systemid = this->identifier.get();
	LevDistance* lev;
	lev = new LevDistance();
	SystemList.insert(std::make_pair(systemid, lev));
	if (search_cmds) {
		for (const auto &cmd : CmdArray) {
			lev->AddWord(cmd);
		}
	}
	return systemid;
}

bool CLevenDis::DelLevelSystem(int systemid)
{
	auto i = SystemList.find(systemid);
	if (i != SystemList.end())
	{
		this->identifier.remove(i->first, SystemList.size());
		delete i->second;
        SystemList.erase(i);
		return true;
	}
	return false;
}

LevDistance * CLevenDis::GetSystem(int systemid)
{
	auto i = SystemList.find(systemid);
	if (i != SystemList.end())
	{
		return i->second;
	}
	return nullptr;
}
int CLevenDis::LevenshteinDistance(string source, string target)
{
	if (source.size() > target.size()) {
		return LevenshteinDistance(target, source);
	}

	const int min_size = source.size(), max_size = target.size();
	std::vector<int> lev_dist(min_size + 1);

	for (int i = 0; i <= min_size; ++i) {
		lev_dist[i] = i;
	}

	for (int j = 1; j <= max_size; ++j) {
		int previous_diagonal = lev_dist[0], previous_diagonal_save;
		++lev_dist[0];

		for (int i = 1; i <= min_size; ++i) {
			previous_diagonal_save = lev_dist[i];
			if (source[i - 1] == target[j - 1]) {
				lev_dist[i] = previous_diagonal;
			}
			else {
				lev_dist[i] = std::min(std::min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
			}
			previous_diagonal = previous_diagonal_save;
		}
	}

	return lev_dist[min_size];
}