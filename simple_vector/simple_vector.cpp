#include "simple_vector.h"
#include "test_runner.h"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void TestConstruction() {
	SimpleVector<int> empty;
	ASSERT_EQUAL(empty.Size(), 0u);
	ASSERT_EQUAL(empty.Capacity(), 0u);
	ASSERT(empty.begin() == empty.end());

	SimpleVector<string> five_strings(5);
	ASSERT_EQUAL(five_strings.Size(), 5u);
	ASSERT(five_strings.Size() <= five_strings.Capacity());
	for (auto& item : five_strings) {
		ASSERT(item.empty());
	}
	five_strings[2] = "Hello";
	ASSERT_EQUAL(five_strings[2], "Hello");
}

void TestPushBack() {
	SimpleVector<int> v;
	for (int i = 10; i >= 1; --i) {
		v.PushBack(i);
		ASSERT(v.Size() <= v.Capacity());
	}
	sort(begin(v), end(v));

	const vector<int> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	ASSERT_EQUAL(v.Size(), expected.size());
	ASSERT(equal(begin(v), end(v), begin(expected)));
}

void TestCopyAssignment() {
	SimpleVector<int> numbers(10);
	iota(numbers.begin(), numbers.end(), 1);

	SimpleVector<int> dest;
	ASSERT_EQUAL(dest.Size(), 0u);

	dest = numbers;
	ASSERT_EQUAL(dest.Size(), numbers.Size());
	ASSERT(dest.Capacity() >= dest.Size());
	ASSERT(equal(dest.begin(), dest.end(), numbers.begin()));
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestConstruction);
	RUN_TEST(tr, TestPushBack);
	RUN_TEST(tr, TestCopyAssignment);
	return 0;
}
