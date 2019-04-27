#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>
class Deque {
public:
	Deque() {}
	bool Empty() const {
		return Size() == 0;
	}

	size_t Size() const{
		return _front.size() + _back.size();
	}

	T& operator[](int i) {
		if (i < _front.size())
			return _front[_front.size() - 1 - i];
		return _back[i - _front.size()];
	}
	const T& operator[](int i) const {
		if (i < _front.size())
			return _front[_front.size() - 1 - i];
		return _back[i - _front.size()];
	}

	T& At(size_t i) {
		if (i<0 || i >= Size()) {
			throw out_of_range("");
		}
		if (i < _front.size())
			return _front[_front.size() - 1 - i];
		return _back[i - _front.size()];
	}
	const T& At(size_t i) const {
		if (i<0 || i >= Size()) {
			throw out_of_range("");
		}
		if (i < _front.size())
			return _front[_front.size() - 1 - i];
		return _back[i - _front.size()];
	}

	T& Front() {
		if (_front.empty()) {
			return _back.front();
		}
		return _front.back();
	}
	const T& Front() const {
		if (_front.empty()) {
			return _back.front();
		}
		return _front.back();
	}
	T& Back() {
		if (_back.empty()) {
			return _front.front();
		}
		return _back.back();
	}
	const T& Back() const {
		if (_back.empty()) {
			return _front.front();
		}
		return _back.back();
	}
	void PushFront(T item) {
		_front.push_back(item);
	}
	void PushBack(T item) {
		_back.push_back(item);
	}
private:
	vector<T> _front;
	vector<T> _back;
};
 
