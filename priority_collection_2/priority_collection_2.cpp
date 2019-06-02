#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <functional>
#include <list>
using namespace std;

template <typename T>
class PriorityCollection {
private:
	struct Item;
	struct ItemRef {
		typename set<Item>::iterator ref;
		bool valid;
	};
	
	struct Item {
		typename list<ItemRef>::iterator id;
		T object;
		int object_order;
		int priority;
		
		Item(typename list<ItemRef>::iterator i_id, T obj, int order, int pr) {
			id = i_id;
			object = move(obj);
			object_order = order;
			priority = pr;
		}

		Item(T obj, int order, int pr) {
			object = move(obj);
			object_order = order;
			priority = pr;
		}

		bool operator<(const Item& other) const {
			if (priority == other.priority)
				return object_order < other.object_order;
			return priority < other.priority;
		}
	};
	set<Item> objects;

	
	list <ItemRef> references;
	ItemRef dummy;
public:
	using Id = typename list<ItemRef>::iterator;

	Id Add(T object) {
		references.push_back(dummy);
		auto tmp = objects.insert(Item{
			prev(references.end()),
			move(object),
			int(references.size() - 1),
			0
			});
		*prev(references.end()) = { tmp.first, true };
 		return prev(references.end());
	}

	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
		IdOutputIt ids_begin) {
		vector<Id> ids;
		for (auto a = range_begin; a != range_end; a= next(a)) {
			ids.push_back(Add(move(*a)));
		}
		copy(ids.begin(), ids.end(), ids_begin);
	}

	bool IsValid(Id id) const {
		return id->valid;
	}

	const T& Get(Id id) const {
		if (IsValid(id))
			return id->ref->object;
	}

	void Promote(Id id) {
		if (!IsValid(id))
			return;
		
		Item item = move(objects.extract(id->ref).value());
		auto insert = objects.insert( Item{
			id,
			move(item.object),
			item.object_order,
			item.priority + 1
			}
		);
		id->ref = insert.first;
	}

	pair<const T&, int> GetMax() const {
		const T& obj = cref(objects.rbegin()->object);
		return { obj, objects.rbegin()->priority };
	}

	pair<T, int> PopMax() {
		auto a = move(objects.extract(prev(objects.end())).value());
		a.id->valid = false;
		return { move(a.object), a.priority };
	}
};


class StringNonCopyable : public string {
public:
	using string::string;
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
	PriorityCollection<StringNonCopyable> strings;
	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");
	
	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) {
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);
	strings.IsValid(yellow_id);
	{
		const auto item = strings.GetMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
	{
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
	strings.IsValid(red_id);
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
	}
	
	vector<StringNonCopyable> str;
	str.push_back("a");
	str.push_back("B");
	str.push_back("c");
	
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestNoCopy);
	return 0;
}
