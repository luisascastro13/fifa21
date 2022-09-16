#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>

//CONSTANTES
#define QJ 18944	//QUANTIDADE DE JOGADORES 	- tabela original: 18944
#define QT 364950 	//QUANTIDADE DE TAGS		- tabela original: 364950
#define QR 24188078	//QUANTIDADE DE RATINGS 	- tabela original: 24188078

//nome dos arquivos
#define PLAYERSFILE "players.csv"
#define RATINGFILE "rating.csv"
#define TAGSFILE "tags.csv"

//ESTRUTURAS
#include "s_players.h"
#include "s_ratings.h"
#include "s_tags.h"

using namespace std;

// PROTOTIPOS DE FUNCOES
void read_players_csv(); 				//le o arquivo players.csv e salva no vetor de structs
void print_players(); 					//exibe a lista dos jogadores (nome id)
void print_playerpos(s_players jog); 	//exibe a lista de posicoes do jogador jog
void read_tags_csv();					//le arquivo tags.csv e salva no vetor de structs
void print_tags();						//exibe a lista de tags (tag_id user_id sofifa_id tag)

//VARIAVEIS GLOBAIS
s_players lista_jogadores[QJ];	//array de jogadores structs
s_ratings lista_ratings[QR];  	//array de ratings 
s_tags lista_tags[QT];			//array de tags
 
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
int main()
{
	//le os jogadores do CSV e exibe a lista de estruturas salvas
	read_players_csv();
	print_players();

	cout << "######################" << endl;

	read_tags_csv();
	print_tags();

	
	return 0;
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//##########################################
//##########    I/O PLAYERS   ##############
//##########################################
//le o arquivo players.csv e salva no vetor de structs
void read_players_csv()
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
					//item.pop_back();	//remove as aspas
					//item.erase( item.end()-1 );
					item.erase(item.end());					
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


//##########################################
//##########    I/O RATINGS	  ##############
//##########################################


//todo

//##########################################
//##########    I/O TAGS	  ##############
//##########################################

void read_tags_csv()
{
    ifstream inFile(TAGSFILE); //our file
    string line;
    int linenum = 0;

    while (getline (inFile, line))
    {
		s_tags t; //temp tag struct for use in the while loop
		
        istringstream linestream(line);
        string item;

		t.tag_id = linenum;
		
		//USER_ID
		getline(linestream, item, ',');
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			stringstream ss(item);
			ss >> t.user_id;
		}

		//SOFIFA_ID
		getline(linestream, item, ',');	//convert to a string stream	
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			stringstream ss(item);
			ss >> t.sofifa_id;
		}

		// //verifica se existem ASPAS. se sim, significa que o array das posicoes tem mais de 1 posicao
		getline(linestream, item, ',');
		t.tag = item;
		//t.tag = item;
		//strcpy(t.tag, item);

		//add the new tag data to the database
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			lista_tags[linenum-1] = t;
		}		

		linenum++;
	}
}

void print_tags(){
	cout 	<< left << setw(10) << "TAG_ID"
			<< left << setw(10) << "USER_ID"
			<< left << setw(10) << "SOFIFA_ID"
			<< left << setw(50) << "TAG"
			<< endl;

	//output the jogadores data.
	for(int i = 0; i < QT; i++) 
	{
		cout 	<< left << setw(10) << lista_tags[i].tag_id
				<< left << setw(10) << lista_tags[i].user_id
				<< left << setw(10) << lista_tags[i].sofifa_id
				<< left << setw(50) << lista_tags[i].tag
				<< endl;
	}
}
