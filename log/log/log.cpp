#include "test_runner.h"
#include <sstream>
#include <string>
using namespace std;

class Logger {
public:
	explicit Logger(ostream& output_stream) : os(output_stream) {
	}

	void SetLogLine(bool value) { log_line = value; }
	void SetLogFile(bool value) { log_file = value; }
	void OutFile(const string& file)
	{
		if (log_file) {
			os << file;
			os << (log_line ? ":" : " ");
		}
	}
	void OutLine(const int line) { 
		if (log_line) {
			if (log_file)
				os << line << " ";
			else
				os << "Line " << line << " ";
		}
	}
	
	void Log(const string& message) {
		os << message << endl;
	}

private:
	ostream& os;
	bool log_line = false;
	bool log_file = false;
};

#define LOG(logger, message) 	\
	logger.OutFile(__FILE__);	\
	logger.OutLine(__LINE__);	\
	logger.Log(message);


void TestLog() {
#line 1 "logger.cpp"

	ostringstream logs;
	Logger l(logs);
	LOG(l, "hello");

	l.SetLogFile(true);
	LOG(l, "hello");

	l.SetLogLine(true);
	LOG(l, "hello");

	l.SetLogFile(false);
	LOG(l, "hello");

	string expected = "hello\n";
	expected += "logger.cpp hello\n";
	expected += "logger.cpp:10 hello\n";
	expected += "Line 13 hello\n";
	ASSERT_EQUAL(logs.str(), expected);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestLog);
}
