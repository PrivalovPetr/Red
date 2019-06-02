#include "test_runner.h"
#include <numeric>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <future>
using namespace std;

template <typename Iterator >
struct IteratorRange {
	Iterator first, last;

	Iterator begin() {
		return first;
	}
	Iterator end() {
		return last;
	}
	const Iterator begin() const {
		return first;
	}
	const Iterator end() const {
		return last;
	}
	size_t size() const {
		return last - first;
	}
};


template <typename Iterator>
class Paginator {
private:
	Iterator _begin, _end;
	size_t _page_size;
	vector<IteratorRange<Iterator>> _pages;
public:
	Paginator<Iterator>(
		Iterator begin,
		Iterator end,
		size_t page_size
		)
		: _begin(begin)
		, _end(end)
		, _page_size(page_size)
	{
		Iterator cur = _begin;
		while (end - cur > 0) {
			auto add = min(page_size, size_t(end - cur));
			_pages.push_back({ cur, next(cur, add) });
			cur += add;
		}
	}

	auto begin() {
		return _pages.begin();
	}
	auto end() {
		return _pages.end();
	}
	size_t size() const {
		return _pages.size();
	}
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
	return Paginator(c.begin(), c.end(), page_size);
}

template <typename C>
int64_t SingleThreadSum(const C& c) {
	int64_t sum = 0;
	for (const auto& row : c) {
		for (const auto& item : row) {
			sum += item;
		}
	}
	return sum;
}
 
int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
	vector<future<int64_t>> result;
	for (auto page : Paginate(matrix, 2250)) {
		result.push_back(async([=] {return SingleThreadSum(page); }));
	}
	int64_t sum = 0;
	for (int i = 0; i < result.size(); ++i) {
		sum += result[i].get();
	}
	return sum;
}

void TestCalculateMatrixSum() {
	const vector<vector<int>> matrix = {
	  {1, 2, 3, 4},
	  {5, 6, 7, 8},
	  {9, 10, 11, 12},
	  {13, 14, 15, 16},
	  {1, 1, 1, 1},
	  {1, 1, 1, 1},
	  {1, 1, 1, 1},
	  {1, 1, 1, 1},
	  {1, 1, 1, 1}
	};
	ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestCalculateMatrixSum);
}
