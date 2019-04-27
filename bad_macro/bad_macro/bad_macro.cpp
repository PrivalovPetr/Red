#include "test_runner.h"

#include <ostream>
using namespace std;

template<typename T1, typename T2>
void print_values(ostream& out, T1 x, T2 y) {
	out << (x) << endl; 
	out << (y) << endl;
}

#define PRINT_VALUES(out, x, y) print_values(out, x, y)

int main() {
	TestRunner tr;
	tr.RunTest([] {
		ostringstream output;
		if (false)
			PRINT_VALUES(output, 5, "red belt");
		else
			PRINT_VALUES(output, 6, "red belt");
		ASSERT_EQUAL(output.str(), "5\nred belt\n");
	}, "PRINT_VALUES usage example");
}
