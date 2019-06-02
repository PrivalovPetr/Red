#include <vector>
#include <algorithm>

using namespace std;

template <class T>
class PriorityCollection {
public:
	using Id = int;

	Id Add(T object) {
		Id id = objects.size();
		objects.push_back(move(object));
		priorities.push_back(0);
		return id;
	}

	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
		IdOutputIt ids_begin) {
		vector<Id> ids;
		for (auto a = range_begin; a != range_end; a = next(a)) {
			ids.push_back(Add(move(*a)));
		}
		copy(ids.begin(), ids.end(), ids_begin);
	}

	bool IsValid(Id id) const {
		if (id >= objects.size())
			return false;
		if (priorities[id] < 0)
			return false;
		return true;
	}

	const T& Get(Id id) const {
		if (IsValid(id))
			return cref(objects[id]);
	}

	void Promote(Id id) {
		if (IsValid(id))
			priorities[id]++;
	}

	pair<const T&, int> GetMax() const {
		Id max_id = MaxId();
		const T& element = cref(objects[max_id]);
		return { element, priorities[max_id] };
	}

	pair<T, int> PopMax() {
		Id max_id = MaxId();
		int priority = priorities[max_id];
		priorities[max_id] = -1;
		return { move(objects[max_id]), priority };
	}

private:
	Id MaxId() const {
		int max = *(max_element(priorities.begin(), priorities.end()));
		for (int i = priorities.size() - 1; i >= 0; --i) {
			if (priorities[i] == max)
				return i;
		}
		return 0;
	}

	vector<T> objects;
	vector<int> priorities;
};
