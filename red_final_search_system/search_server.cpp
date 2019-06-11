#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <future>
#include <mutex>
#include <functional>
#include <deque>

vector<string> SplitIntoWords(string&& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

vector<string> SplitIntoWords(const string& line) {
	istringstream words_input(line);
	return { istream_iterator<string>(words_input), istream_iterator<string>() };
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	future<InvertedIndex> new_index = async([&document_input]
	{
		InvertedIndex new_index;
		for (string current_document; getline(document_input, current_document); ) {
			new_index.Add(move(current_document));
		}
		return new_index;
	});
	
	lock_guard<mutex> guard(index_mutex);
	index = move(new_index.get());
}

void SearchServer::AddQueriesStreamSingleThread(
  istream& query_input, ostream& search_results_output
) {
	vector<size_t> docid_count(index.GetDocumentsCount());
  for (string current_query; getline(query_input, current_query); ) {
	const auto words = SplitIntoWords(move(current_query));

	fill(docid_count.begin(), docid_count.end(), 0);
	{
		lock_guard<mutex> lg(index_mutex);
		for (const auto& word : words) {
			for (const auto& pair : index.Lookup(word)) {
				docid_count[pair.first] += pair.second;
			}
		}
	}

	search_results_output << current_query << ':';
	for (int i = 0; i < 5; i++) {
		auto m = max_element(docid_count.begin(), docid_count.end());
		size_t ind = m - docid_count.begin();
		if (docid_count[ind] == 0) {
			break;
		}
		else {
			search_results_output << " {"
				<< "docid: " << ind << ", "
				<< "hitcount: " << docid_count[ind] << '}';
			docid_count[ind] = 0;
		}
 	}
    search_results_output << endl;
  }
}

void SearchServer::AddQueriesStream(
	istream& query_input, ostream& search_results_output
) {
	multitread.push_back(
		//async(this->AddQueriesStreamSingleThread, &query_input, &search_results_output);
		async([&query_input, &search_results_output, this]
		{
			this->AddQueriesStreamSingleThread(ref(query_input), ref(search_results_output));
		}
	));
	//this_thread::sleep_for(chrono::microseconds(1000));
}

void InvertedIndex::Add(const string& document) {
	if (docs.size() == 0) {
		docs.reserve(50000);
	}
	docs.emplace_back(document);

  const size_t docid = docs.size() - 1;
  map<string, size_t> tmp;
  for (const auto& word : SplitIntoWords(document)) {
	  ++tmp[word];
  }
  for (const auto& pair : tmp) {
	  index[move(pair.first)].push_back({docid, pair.second});
  }
  
}

const vector < pair<size_t, size_t>> empty_vector;

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty_vector;
  }
}
