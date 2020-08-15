#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;
struct trie
{
	typedef std::shared_ptr<trie> trieptr;
	typedef map<char, trieptr> next_t;

	// The set with all the letters which this node is prefix
	next_t next;

	// If word is equal to "" is because there is no word in the
	//  dictionary which ends here.
	string word;

	trie() : next(map<char, trieptr>()) {}
	~trie()
	{
	}
	bool insert(string w)
	{
		w = string("$") + w;
		int sz = w.size();
        bool success = false;
		trie* n = this;
		for (int i = 0; i < sz; ++i) {
			if (n->next.find(w[i]) == n->next.end()) {
				n->next[w[i]] = trieptr(new trie());
                success = true;
			}

			n = n->next[w[i]].get();
		}
		n->word = w;
        return success;
	}
	bool remove(string w)
	{
		w = string("$") + w;
		int sz = w.size();
		trie* n = this;
		trie* lastroot = this;
		char last = '$';
		for (int i = 0; i < sz; ++i) {
			if (n->next.find(w[i]) != n->next.end()) {
				n = n->next[w[i]].get();
				if (n->next.size() > 1) {
					lastroot = n;
					last = w[i];
				}
			}
		}
		if (n->word == w)
		{
			n->word = "";
			if (!n->next.size()) {
				lastroot->next.erase(last);
			}
			return true;
		}
		return false;
	}
};
class LevDistance
{
private:
	trie tree;
	void search_rec(trie tree2, char ch, vector<int> last_row, const string& word);
	int min_cost;
	string foundword;
	int word_count;
public:
	LevDistance();
	~LevDistance();
	bool AddWord(string word);
	bool DelWord(string word);
	int Search(string word);
	string GetFoundWord();
	int GetWordCount();
};