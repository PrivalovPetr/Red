#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
private:
	set<string> dict;

public:
	int Learn(const vector<string>& words) {
		int size = dict.size();
		dict.insert(words.begin(), words.end());
		return dict.size()-size;
	}

	vector<string> KnownWords() {
		vector<string> result(dict.begin(), dict.end());
		return result;
	}
};

