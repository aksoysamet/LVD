#include "clvd.h"
#include "identifier.h"
#include <unordered_map>
class CLevenDis
{
private:
	std::unordered_map<int, LevDistance*> SystemList;
	Identifier identifier;
public:
	std::vector<std::string>CmdArray;
	CLevenDis();
	~CLevenDis();
	int AddLevenSystem(bool search_cmds);
	bool DelLevelSystem(int systemid);
	LevDistance* GetSystem(int systemid);
	int LevenshteinDistance(string source, string target);
};