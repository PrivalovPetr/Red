#include "test_runner.h"

using namespace std;

template <typename T>
class Deque {
public:
	Table(size_t n, size_t m) {
		Resize(n, m);
	}
	vector<T>& operator[](int i) {
		return _table[i];
	}
	vector<T> operator[](int i) const {
		return _table[i];
	}
	void Resize(size_t n, size_t m) {
		_table.resize(n);
		for (vector<T>& v : _table) {
			v.resize(m);
		}
	}
	pair<size_t, size_t> Size() const {
		return make_pair(_table.size(), _table[0].size());
	}
private:
	vector<vector<T>> _table;
};

void TestTable() {
	Table<int> t(1, 1);
	ASSERT_EQUAL(t.Size().first, 1u);
	ASSERT_EQUAL(t.Size().second, 1u);
	t[0][0] = 42;
	ASSERT_EQUAL(t[0][0], 42);
	t.Resize(3, 4);
	ASSERT_EQUAL(t.Size().first, 3u);
	ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestTable);
	return 0;
}
