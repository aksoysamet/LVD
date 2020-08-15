#include "clvd.h"
typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;
LevDistance::LevDistance()
{
	word_count = 0;
}
LevDistance::~LevDistance()
{
}
bool LevDistance::AddWord(string word)
{
	bool success;
	try {
		success = this->tree.insert(word);
	}
	catch (exception e)
	{
		logprintf("[LVD][Exception] AddWord(%s): %s", word.c_str(), e.what());
	}
    if (success)
	{
		word_count++;
	}	
	return success;
}

bool LevDistance::DelWord(string word)
{
	bool success;
	try {
		success = this->tree.remove(word);
	}
	catch (exception e)
	{
		logprintf("[LVD][Exception] DelWord(%s): %s", word.c_str(), e.what());
	}
	if (success)
	{
		word_count--;
	}
	return success;
}
int LevDistance::GetWordCount()
{
	return this->word_count;
}
string LevDistance::GetFoundWord()
{
	return this->foundword;
}

int LevDistance::Search(string word)
{
	if (!GetWordCount())
	{
		logprintf("There is no word in dictionary!");
		return -1;
	}
	try {
		word = string("$") + word;
		int sz = word.size();
		min_cost = 0x3f3f3f3f;
		vector<int> current_row(sz + 1);
		// Naive DP initialization
		for (int i = 0; i < sz; ++i) current_row[i] = i;
		current_row[sz] = sz;
		foundword = "";
		// For each letter in the root map wich matches with a
		//  letter in word, we must call the search
		for (int i = 0; i < sz; ++i) {
			if (tree.next.find(word[i]) != tree.next.end()) {
				this->search_rec(*tree.next[word[i]], word[i], current_row, word);
			}
		}
		if (foundword.size() > 1)foundword = foundword.substr(1, foundword.length());
	}
	catch (exception e)
	{
		logprintf("[LVD][Exception] Search(%s): %s", word.c_str(), e.what());
	}
	return min_cost;
}
void LevDistance::search_rec(trie tree2, char ch, vector<int> last_row, const string& word)
{
	int sz = last_row.size();
	vector<int> current_row(sz);
	current_row[0] = last_row[0] + 1;

	// Calculate the min cost of insertion, deletion, match or substution
	int insert_or_del, replace;
	for (int i = 1; i < sz; ++i) {
		insert_or_del = min(current_row[i - 1] + 1, last_row[i] + 1);
		replace = (word[i - 1] == ch) ? last_row[i - 1] : (last_row[i - 1] + 1);

		current_row[i] = min(insert_or_del, replace);
	}
	// When we find a cost that is less than the min_cost, is because
	// it is the minimum until the current row, so we update
	if ((current_row[sz - 1] < min_cost) && (tree2.word != "")) {
		min_cost = current_row[sz - 1];
		foundword = tree2.word;
	}
	// If there is an element wich is smaller than the current minimum cost,
	//  we can have another cost smaller than the current minimum cost
	if (*min_element(current_row.begin(), current_row.end()) < min_cost) {
		for (trie::next_t::iterator it = tree2.next.begin(); it != tree2.next.end(); ++it) {
			this->search_rec(*it->second, it->first, current_row, word);
		}
	}
}
