#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>

//QUANTIDADE DE JOGADORES
//tabela original: 18944
#define QJ 18944
#define PLAYERSFILE "players.csv"
#define RATINGFILE "rating.csv"
#define TAGSFILE "tags.csv"

//estruturas
#include "s_players.h"
#include "s_ratings.h"
#include "s_tags.h"

using namespace std;

//FUNCOES
//le o arquivo players.csv e salva no vetor de structs
int read_players_csv();
//exibe a lista dos jogadores (nome id)
void print_players();
//exibe a lista de jogadores com 
void print_playerpos(s_players jog);


 //array of jogadores structs
s_players lista_jogadores[QJ];
 
int main()
{
	read_players_csv();

	print_players();
	
	return 0;
}

//le o arquivo players.csv e salva no vetor de structs
int read_players_csv()
{

    ifstream inFile(PLAYERSFILE); //our file
    string line;
    int linenum = 0;

    while (getline (inFile, line))
    {

		s_players jogador; //temp jogador struct for use in the while loop
		vector<string> pp;

        istringstream linestream(line);
        string item;
		
		//ID 
		getline(linestream, item, ',');
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			stringstream ss(item);
			ss >> jogador.sofifa_id;
		}

		//NOME
		getline(linestream, item, ',');	//convert to a string stream	
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			jogador.name = item;
		}

		// //verifica se existem ASPAS. se sim, significa que o array das posicoes tem mais de 1 posicao
		getline(linestream, item, ',');
		
		if(item.front() == '"'){ //o jogador tem mais de uma posicao

			item.erase(item.begin());	//remove as aspas		
			pp.push_back(item);		 //pushback no vetor

			do{
				getline(linestream, item, ',');
				
				if (item.back() == '"'){
					item.erase(item.end());	//remove as aspas
				}
				pp.push_back(item);

			}while(item.back() != '"');

		// 	// //le ate proxima virgula ate encontrar aspas no final do item
		}
		// //caso contrario, so existe 1 posicao.	
		else{
			pp.push_back(item);
		}
		
		jogador.player_positions = pp;		

		//add the new jogador data to the database
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			lista_jogadores[linenum-1] = jogador;
		}		

		linenum++;
	}	

    return 0;
}


void print_players(){
	cout 	<< left << setw(7)  << "N"
			<< left << setw(50) << "NOME"
			<< left << setw(15) << "ID"
			<< left << setw(15) << "POSICOES"
			<< endl;

	//output the jogadores data.
	for(int i = 0; i < QJ; i++) 
	{
		cout 	<< left << setw(7)   << i+1
				<< left << setw(50)  << lista_jogadores[i].name
				<< left << setw(15)  << lista_jogadores[i].sofifa_id;
		print_playerpos(lista_jogadores[i]);
		cout << endl;
	}
}


void print_playerpos(s_players jog){
	vector<string> pp = jog.player_positions;
	//output the players positions.	
	for(int j =0 ; j < pp.size();j++){
		cout << pp[j];
	}

}
