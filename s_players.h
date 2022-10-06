#ifndef PLAYERS
#define PLAYERS

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

template<class C, typename T>
bool contains(C&& c, T t) {
    return find(begin(c), end(c), t) != end(c);
};

using namespace std;
struct s_players{
	int  sofifa_id;
	string name;
	vector <string> player_positions;
	vector<string> player_tags;
	float rating;
	int count;
};

void print_players(s_players p){
	cout << p.sofifa_id << " " << p.name;
}

//retorna uma lista dos players que tem count >= valor
vector<s_players> getPlayersMaiorMil(vector<s_players> lista_jogadores, int valor){
	vector<s_players> res;
	for (int x=0;x<QJ;x++){
		if (lista_jogadores[x].count >= valor){		
			res.push_back(lista_jogadores[x]);
		}
		//cout << x << "\n";
	}
	return res;

}

vector<s_players> getPlayerWithPosition(vector<s_players> p, string posicao){
	vector<s_players> res;
	for(int x=0; x<p.size(); x++){		
		bool exists = contains(p[x].player_positions,posicao);
		if(exists){
			res.push_back(p[x]);
		}
	}
	return res;
}

vector<string> getPlayerTags(s_players p){
	return p.player_tags;
}


vector<s_players> getPlayersWithTags(vector<s_players> p, vector<string> tags){
	vector<s_players> res;
	for(int x=0; x<p.size(); x++){		
		//INCOMPLETO AINDA!!!
		cout << "\nopa! x : " << x;
		for(int a=0; a<tags.size(); a++){
			cout << "\nopa! a: " << a;
			//bool exists = contains(p[x].player_tags,tags[a]);
			//bool exists = count(begin(p[x].player_tags), end(p[x].player_tags), tags[a]) > 0;
			// // bool exists = false;
			// // for (int i: p[x].player_tags[a]){
			// // 	if (i == tags[a]) {
			// // 		exists = true;
			// // 		break;
			// // 	}
			// // }
			// if(exists){
			// 	res.push_back(p[x]);
			// }
		}
		
		//bool exists = count(:begin(arr), std::end(arr), target) > 0;

	}
	return res;
}
//funcao que verifica se existe 1 tag no player
bool playerHasTag(s_players p, string tag){
	if(contains(p.player_tags, tag)){
		return true;
	}
	else{
		return false;	
	}
}

// funcao que verifica se player tem N tags
bool playerHasAllTags(s_players p, vector<string> tags){
	bool hasAll = true;
	for (int x=0;x<tags.size();x++){
		if (!playerHasTag(p, tags[x])){
			hasAll = false;
		}
	}
	return hasAll;
}

//funcao que busca todos usuarios que tem as N tags
vector<s_players> playersWithAllTags(vector<s_players> all, vector<string> tags){
	vector<s_players> res;
	
	for(int y =0;y<all.size();y++){
		if (playerHasAllTags(all[y],tags)){
			res.push_back(all[y]);
		}
	}
	return res;
		
}


void ordenaPlayersPorRating(vector<s_players>& vet){ //shell sort
	int h=1;
	while (h< vet.size()/3){
		h = 3*h+1;//knuth
	} 
    int pos, auxpos;
	s_players num;
	while (h>0){
		for (int pos=h;pos<vet.size();pos++){//começa comparando numero na posição h
			num = vet[pos]; //numero na posição pos

			auxpos = pos; //salva posição atual

			//enquanto posição>= ao tamanho do "pulo" h &&
			//o número na posição pos for maior que o número na posição auxpos-h
			while (auxpos>=h && num.rating >vet[auxpos-h].rating){

				vet[auxpos] = vet[auxpos-h]; //troca de posição
				auxpos = auxpos - h;
			}
			vet[auxpos] = num;
		}
		h= (h-1)/3; //knuth
    }		
}
#endif /*PLAYERS*/