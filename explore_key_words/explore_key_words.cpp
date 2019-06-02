#include "test_runner.h"
#include "profile.h"

#include <future>
#include <functional>
#include <map>
#include <string>
#include <set>
#include <sstream>
#include <iterator>
#include <iostream>
using namespace std;

struct Stats {
	map<string, int> word_frequences;

	void operator += (const Stats& other) {
		for (const auto& pair : other.word_frequences) {
			if (word_frequences.count(pair.first) > 0) {
				word_frequences[pair.first] += pair.second;
			}
			else {
				word_frequences[pair.first] = pair.second;
			}
		}
	}
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
	Stats stats;
	size_t left = 0;
	while (left < line.size()) {
		size_t r = line.find(" ", left);
		if (r == -1) {
			r = line.size();
		}
		auto word = line.substr(left, r-left);
		if (key_words.count(word) > 0) {
			++stats.word_frequences[word];
		}
		left = r + 1;		
	}
	return stats;
}

Stats ExploreKeyWordsSingleThread(
	const set<string>& key_words, istream& input
) {
	Stats result;
	for (string line; getline(input, line); ) {
		result += ExploreLine(key_words, line);
	}
	return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	vector<future<Stats>> stats;
	vector<stringstream> streams;
	for (int k = 0; k < 8; ++k) {
		streams.push_back(stringstream());
		stats.push_back({});
	}
	stats.reserve(8);
	int i = 0;
	for (string line; getline(input, line);)
	{
		streams[i++] << line << " ";
		if (i == 8)
			i = 0;
	}
	for(i = 0; i < 8 ; ++i)
		stats[i] = async(ExploreKeyWordsSingleThread, ref(key_words), ref(streams[i]));

	Stats res;
	for(auto & stat : stats)
	{
		res += stat.get();
	}

	return res;
}

void TestBasic() {
	const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

	ExploreLine(key_words, "yangle sucks google-rocks digity-po");

	stringstream ss;
	ss << "this new yangle service really rocks\n";
	ss << "It sucks when yangle isn't available\n";
	ss << "10 reasons why yangle is the best IT company\n";
	ss << "yangle rocks others suck\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
	ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

	const auto stats = ExploreKeyWords(key_words, ss);
	const map<string, int> expected = {
	  {"yangle", 16},
	  {"rocks", 2},
	  {"sucks", 1}
	};
	ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestBasic);
}
