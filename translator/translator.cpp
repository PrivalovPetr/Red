#include "test_runner.h"
#include <string>
#include <string_view>
#include <deque>


using namespace std;

class Translator {
public:
	void Add(string_view source, string_view target) {
		auto s_pointer = SearchOrPushBack(source);
		auto t_pointer = SearchOrPushBack(target);
		
		_forward[s_pointer] = t_pointer;
		_backward[t_pointer] = s_pointer;
	}
	string_view TranslateForward(string_view source) const {
		return Find(&_forward, source);
	}
	string_view TranslateBackward(string_view target) const {
		return Find(&_backward, target);
	}


private:
	string_view SearchOrPushBack(string_view str) {
		for (const auto * m_pointer : { &_forward, &_backward }) {
			const auto iter = m_pointer->find(str);
			if (iter != m_pointer->end()) {
				return iter->first;
			}
		}
		_data.push_back(string(str));
		return _data[_data.size() - 1];
	}

	string_view Find(const map<string_view, string_view>* map, string_view item) const {
		const auto it = map->find(item);
		if (it == map->end()) {
			return {};
		}
		return it->second;
	}

	deque<string> _data;
	map<string_view, string_view> _forward;
	map<string_view, string_view> _backward;
};

void TestSimple() {
	Translator translator;
	translator.Add(string("okno"), string("window"));
	translator.Add(string("stol"), string("table"));

	ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
	ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
	ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSimple);
	return 0;
}
