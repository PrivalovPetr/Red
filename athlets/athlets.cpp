#include <iostream>
#include <list>
#include <vector>

using namespace std;
int main()
{
	int n;
	cin >> n;
	list<int> athlets;
	vector<list<int>::iterator> pointers(100'001, athlets.end());
	for (int i = 0; i < n; ++i) {
		int num;
		int prev;
		cin >> num >> prev;
		auto pos =	 pointers[prev];
		pointers[num] = athlets.insert(pos, num);
	}
	for (auto a : athlets) {
		cout << a << " ";
	}
}

