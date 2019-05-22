#include "stats.h"

Stats::Stats() {
	for (auto a : methods) {
		method_counter[a] = 0;
	}
	for (auto a : uris) {
		uri_counter[a] = 0;
	}
}

string_view Stats::GetMethodsView(string_view s) {
	for (int i = 0; i < 4; ++i) {
		if (s == methods[i])
			return methods[i];
	}
	return methods[4];
}

string_view Stats::GetUrisView(string_view s) {
	for (int i = 0; i < 5; ++i) {
		if (s == uris[i])
			return uris[i];
	}
	return uris[5];
}


void Stats::AddMethod(string_view method)
{
	string_view meth = GetMethodsView(method);
	++method_counter[meth];
}

void Stats::AddUri(string_view uri)
{
	string_view u = GetUrisView(uri);
	++uri_counter[u];
}

const map<string_view, int>& Stats::GetMethodStats() const
{
	return method_counter;
}

const map<string_view, int>& Stats::GetUriStats() const
{
	return uri_counter;
}

string_view GetWord(string_view& line) {
	auto space_pos = line.find(" ");
	string_view word = line.substr(0, space_pos);
	line.remove_prefix(space_pos + 1);
	return word;
}

HttpRequest ParseRequest(string_view line)
{
	line.remove_prefix(line.find_first_not_of(" "));
	
	auto method = GetWord(line);
	auto uri = GetWord(line); 
	return { method, uri, line };
}
