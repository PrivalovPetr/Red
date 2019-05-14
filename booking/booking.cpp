#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <stdint.h>

using namespace std;

struct Booking {
	int64_t time;
	string hotel_name;
	int client_id;
	int room_count;
};

class BookManager {
public:
	void Book(int64_t time, string hotel_name, int client_id, int room_count) {
		Booking b{ time, hotel_name, client_id, room_count };
		current_time = time;
		last_day.push(b);
		++last_day_clients[hotel_name][client_id];
		last_day_rooms[hotel_name] += room_count;
		AdjustLastDay();
	}

	int Clients(string hotel_name) {
		AdjustLastDay();
		return last_day_clients[hotel_name].size();
	}

	int Rooms(string hotel_name) {
		AdjustLastDay();
		return last_day_rooms[hotel_name];
	}
private:
	int64_t current_time;
	queue<Booking> last_day;
	map<string, map<int, int>> last_day_clients;
	map<string, int> last_day_rooms;
	
	void AdjustLastDay() {
		while (!last_day.empty() && last_day.front().time <= current_time - 86400) {
			const Booking& b = last_day.front();
			last_day_rooms[b.hotel_name] -= b.room_count;
			--last_day_clients[b.hotel_name][b.client_id];
			if (last_day_clients[b.hotel_name][b.client_id] == 0) {
				last_day_clients[b.hotel_name].erase(b.client_id);
			}
 			last_day.pop();
		}
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	BookManager manager;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;
		
		if (query_type == "BOOK") {
			uint64_t time;
			cin >> time;
			string hotel_name;
			cin >> hotel_name;
			int client_id;
			cin >> client_id;
			int room_count;
			cin >> room_count;
			manager.Book(time, hotel_name, client_id, room_count);
		}
		else if (query_type == "CLIENTS") {
			string hotel_name;
			cin >> hotel_name;
			cout << manager.Clients(hotel_name) << "\n";
		}
		else if (query_type == "ROOMS") {
			string hotel_name;
			cin >> hotel_name;
			cout << manager.Rooms(hotel_name) << "\n";
		}
	}
	return 0;
}
