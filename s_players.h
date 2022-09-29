#ifndef PLAYERS
#define PLAYERS

#include <string>
#include <vector>

using namespace std;
struct s_players{
	int  sofifa_id;
	string name;
	vector <string> player_positions;
	int  rating =0;
	int  count=0;
};

void print_players(s_players p){
	cout << p.sofifa_id << " " << p.name;
}

#endif /*PLAYERS*/