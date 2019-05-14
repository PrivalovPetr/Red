#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
	ObjectPool() {
		_freed = new queue<T*>;
		_allocated = new set<T*>;
	}

	T* Allocate() {
		T* buf = GetPointer(true);
		_allocated->insert(buf);
		return buf;
	}

	T* TryAllocate() {
		T* buf = GetPointer(false);
		_allocated->insert(buf);
		return buf;
	}

	void Deallocate(T* object) {
		if (_allocated->count(object) == 0)
			throw invalid_argument("");
		_allocated->erase(object);
		_freed->push(object);
	}

	~ObjectPool() {
		while (!_freed->empty()) {
			delete _freed->front();
			_freed->pop();
		}			
		delete _freed;
		for (auto a : *_allocated)
			delete a;
		delete _allocated;
	}

private:
	T* GetPointer(bool flag) {
		if (!_freed->empty()) {
			T* buf = _freed->front();
			_freed->pop();
			return buf;
		}
		else {
			if (flag) {
				return new T;
			}
			return nullptr;
		}
		
	}
	queue<T*>* _freed;
	set<T*>* _allocated;
};

void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}


/*
int main() {
	TestRunner tr;
	RUN_TEST(tr, TestObjectPool);
	return 0;
}
*/
