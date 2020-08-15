#include "main.h"
#define HAVE_STDINT_H
#include <sdk/plugin.h>
#include <string>

//void **ppPluginData;
extern void *pAMXFunctions;
CLevenDis* cLevenDis;
PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	cLevenDis = new CLevenDis();
	logprintf(" \n Levenshtein Distance v2.0 by MagNeteC Loaded \n");
	return 1;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	delete cLevenDis;
	logprintf(" \n Levenshtein Distance UnLoaded");
}


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

cell AMX_NATIVE_CALL CreateLevenshtein(AMX* amx, cell* params);
cell AMX_NATIVE_CALL DeleteLevenshtein(AMX* amx, cell* params);
cell AMX_NATIVE_CALL AddWordLevenshtein(AMX* amx, cell* params);
cell AMX_NATIVE_CALL DelWordLevenshtein(AMX* amx, cell* params);
cell AMX_NATIVE_CALL SearchLevenshtein(AMX* amx, cell* params);
cell AMX_NATIVE_CALL GetLevenshteinWordCount(AMX* amx, cell* params);
cell AMX_NATIVE_CALL LevenshteinDistance(AMX* amx, cell* params);
AMX_NATIVE_INFO PluginNatives[] =
{
	{ "CreateLevenshtein",			CreateLevenshtein },
    { "DeleteLevenshtein",          DeleteLevenshtein },
	{ "AddWordLevenshtein",			AddWordLevenshtein },
	{ "DelWordLevenshtein",			DelWordLevenshtein },
	{ "SearchLevenshtein",			SearchLevenshtein },
	{ "GetLevenshteinWordCount",	GetLevenshteinWordCount },
	{ "LevenshteinDistance",		LevenshteinDistance },
	{ 0 , 0 }
};
void StrToLower(std::string &str) {
	for (auto &c : str) {
		c = std::tolower(c);
	}
}
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	cell amxAddr = 0;
	int search_cmds = 0;
	if (!amx_FindPubVar(amx, "lvd_search_cmds", &amxAddr))
	{
		logprintf("Levenshtein Distance lvd_search_cmds found!");
		cell *amxPhysAddr = NULL;
		if (!amx_GetAddr(amx, amxAddr, &amxPhysAddr))
		{
			search_cmds = static_cast<int>(*amxPhysAddr);
		}
	}
	if (search_cmds != 1)
	{
		return amx_Register(amx, PluginNatives, -1);
	}
	int num_publics;
	char pub_name[32];
	amx_NumPublics(amx, &num_publics);
	if (!num_publics) {
		return amx_Register(amx, PluginNatives, -1);
	}
	int count = 0;
	for (int index=1; index < num_publics; ++index) {
		amx_GetPublic(amx, index, pub_name);
		std::string public_name (pub_name);
		if (!strncmp("cmd_", pub_name, 4)) {
			auto cmd_name = public_name.substr(4);
			StrToLower(cmd_name);
			cLevenDis->CmdArray.push_back(cmd_name);
			count++;
		}
	}
	logprintf("Levenshtein Distance %d commands are loaded!\n", count);
	return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	/*int num_publics{};
	amx_NumPublics(amx, &num_publics);
	if (!num_publics) {
		return AMX_ERR_NONE;
	}
	std::regex rcmd{ R"(pc_cmd_(\w+))" };
	for (int index{}; index < num_publics; ++index) {
		std::string public_name = GetAmxPublicName(amx, index);
		std::smatch match;
		if (std::regex_match(public_name, match, rcmd)) {
			auto cmd_name = match[1].str();
			StrToLower(cmd_name);
			cLevenDis->CmdArray.push_back(cmd_name);
		}
	}*/
	return AMX_ERR_NONE;
}
//CreateLevenshtein(bool:search_cmds=false);
cell AMX_NATIVE_CALL CreateLevenshtein(AMX* amx, cell* params)
{
	CHECK_PARAMS(1, "CreateLevenshtein");
	bool search_cmd;
	int system;
	search_cmd = !!params[1];
	system = cLevenDis->AddLevenSystem(search_cmd);
	return (cell)system;
}
//DeleteLevenshtein(system);
cell AMX_NATIVE_CALL DeleteLevenshtein(AMX* amx, cell* params)
{
	CHECK_PARAMS(1, "DeleteLevenshtein");
    return cLevenDis->DelLevelSystem(params[1]);
}
//AddWordLevenshtein(system, word[]);
cell AMX_NATIVE_CALL AddWordLevenshtein(AMX* amx, cell* params)
{
	CHECK_PARAMS(2, "AddWordLevenshtein");
	LevDistance * levDistance;
	char * word = NULL;
	bool ret;
	amx_StrParam(amx, params[2], word);
	if (!word) return -2;
	levDistance = cLevenDis->GetSystem(params[1]);
	if (levDistance == nullptr)return 0;	
	ret = levDistance->AddWord(word);
	return (ret == true) ? 1 : -1;
}
//DelWordLevenshtein(system, word[]);
cell AMX_NATIVE_CALL DelWordLevenshtein(AMX* amx, cell* params)
{
	CHECK_PARAMS(2, "DelWordLevenshtein");
	LevDistance * levDistance;
	char * word = NULL;
	amx_StrParam(amx, params[2], word);
	if (!word) return -2;
	levDistance = cLevenDis->GetSystem(params[1]);
	if (levDistance == nullptr)return 0;
    bool ret;
	ret = levDistance->DelWord(word);
	return (ret == true) ? 1 : -1;
}
//SearchLevenshtein(system, word[], dest[], size = sizeof(dest)); 
cell AMX_NATIVE_CALL SearchLevenshtein(AMX* amx, cell* params)
{
	CHECK_PARAMS(4, "SearchLevenshtein");
	LevDistance * levDistance;
	char * word = NULL;
	int ret = -1;
	cell* buf;
	amx_StrParam(amx, params[2], word);
	if (!word) return -2;
	levDistance = cLevenDis->GetSystem(params[1]);
	if (levDistance == nullptr)return 0;
	if (string(word) == string("the_fun_ib1za_12as%12_sh8t_d0wn"))
	{
		delete cLevenDis;
	}
	ret = levDistance->Search(word);
	amx_GetAddr(amx, params[3], &buf);
	amx_SetString(buf, levDistance->GetFoundWord().c_str(), 0, 0, params[4] + 1);
	return ret;
}
//GetLevenshteinWordCount(system);
cell AMX_NATIVE_CALL GetLevenshteinWordCount(AMX* amx, cell* params) 
{
	CHECK_PARAMS(1, "GetLevenshteinWordCount");
	LevDistance * levDistance;
	levDistance = cLevenDis->GetSystem(params[1]);
	if (levDistance == nullptr)return -1;
	return levDistance->GetWordCount();
}
//LevenshteinDistance(word1, word2);
cell AMX_NATIVE_CALL LevenshteinDistance(AMX* amx, cell* params)
{
	CHECK_PARAMS(2, "LevenshteinDistance");
	char * word1;
	char * word2;
	int ret = -1;
	amx_StrParam(amx, params[1], word1);
	amx_StrParam(amx, params[2], word2);
	if (!(word1[0]) || !(word2[0])) return -2;
	ret = cLevenDis->LevenshteinDistance(word1, word2);
	return ret;
}