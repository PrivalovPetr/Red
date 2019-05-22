#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <array>
using namespace std;

class Stats {
public:
	void AddMethod(string_view method);
	void AddUri(string_view uri);
	const map<string_view, int>& GetMethodStats() const;
	const map<string_view, int>& GetUriStats() const;
	Stats();
private:
	
	string_view GetMethodsView(string_view s);
	string_view GetUrisView(string_view s);
	map<string_view, int> method_counter;
	map<string_view, int> uri_counter;
	const array<string_view, 5> methods = {"GET", "PUT", "POST", "DELETE", "UNKNOWN"};
	const array<string_view, 6> uris = { "/", "/order", "/product", "/basket", "/help", "unknown" };
};

HttpRequest ParseRequest(string_view line);
