#pragma once

#include <cstdlib>

template <typename T>
class SimpleVector {
public:
	SimpleVector() {
		data = nullptr;
		end_ = nullptr;
		top = end_;
	}

	explicit SimpleVector(size_t size) {
		Init(size);
		top = data + size;
	}
	~SimpleVector() {
		delete[] data;
	}

	T& operator[](size_t index) {
		return data[index];
	}

	T* begin() { return data; }
	T* end() { return top; }

	size_t Size() const {
		return top - data;
	}
	size_t Capacity() const {
		return end_ - data;
	}
	void PushBack(const T& value) {
		if (data == nullptr) {
			Init(1);
			top = data;
		}
		if (top == end_) {
			T* buf = data;
			size_t buf_size = Capacity();
			Init(2 * buf_size);
			top = data + buf_size;
			for (size_t i = 0; i < buf_size; ++i) {
				data[i] = buf[i];
			}
			delete[] buf;
		}
		*top = value;
		++top;
	}

private:
	T* data;
	T* end_;
	T* top;
	void Init(size_t size) {
		data = new T[size];
		end_ = data + size;
	}
};
