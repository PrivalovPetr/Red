#include "Deque.h"
#include <iostream>

int main() {
	Deque<int> d;
	d.PushBack(22);
	d.PushFront(12);
	cout << d[1] << endl;
	cout << d.Back();
	return 0;
}
