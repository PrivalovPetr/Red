#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <iterator>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	// 1
	int size = range_end - range_begin;
	if (size < 2)
		return;
	// 2
	vector<typename RandomIt::value_type> items(
		make_move_iterator(range_begin),
		make_move_iterator(range_end)
	);
	// 3
	auto range_1third = items.begin() + size / 3;
	auto range_2third = items.begin() + 2 * size / 3;
	//// 4
	MergeSort(begin(items), range_1third);
	MergeSort(range_1third, range_2third);
	MergeSort(range_2third, end(items));
	//5
	vector<typename RandomIt::value_type> tmp;
	tmp.reserve(2 * size / 3);
	merge(
		make_move_iterator(items.begin()),
		make_move_iterator(range_1third),
		make_move_iterator(range_1third),
		make_move_iterator(range_2third),
		back_inserter(tmp)
	);
	//6	
	merge(
		make_move_iterator(tmp.begin()),
		make_move_iterator(tmp.end()),
		make_move_iterator(range_2third),
		make_move_iterator(items.end()),
		range_begin
	);
}

void TestIntVector() {
	vector<int> numbers = { 6, 1, 3, 9, 1, 9, 8, 12, 1 };
	MergeSort(begin(numbers), end(numbers));
	ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestIntVector);
	return 0;
}
