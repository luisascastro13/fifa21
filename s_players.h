#include <string>
#include <vector>

using namespace std;
struct s_players{
	int  sofifa_id;
	string name;
	vector <string> player_positions;
	int  rating;
	int  count;
};

void print_players(s_players p){
	cout << p.sofifa_id << " " << p.name;
}

