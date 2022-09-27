#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>

//CONSTANTES
#define QJ 18944	//QUANTIDADE DE JOGADORES 	- tabela original: 18944
#define QR 10000	//QUANTIDADE DE RATINGS 	- tabela original: 24188078       MINIRATING: 10000
#define QT 364950 	//QUANTIDADE DE TAGS		- tabela original: 364950

//nome dos arquivos
#define PLAYERSFILE "players.csv"
#define RATINGSFILE "minirating.csv"
#define TAGSFILE "tags.csv"

std::string playersFile()
{
    static std::string word("players.csv");
    return word;
}

//ESTRUTURAS
#include "s_players.h"
#include "s_ratings.h"
#include "s_tags.h"

#include "trie.cpp"

using namespace std;

// PROTOTIPOS DE FUNCOES
void read_players_csv(); 				//le o arquivo players.csv e salva no vetor de structs
void print_players(int qtd); 					//exibe a lista dos jogadores (nome id)
void print_playerpos(s_players jog); 	//exibe a lista de posicoes do jogador jog
void read_ratings_csv();
void print_ratings();

void read_tags_csv();					//le arquivo tags.csv e salva no vetor de structs
void print_tags();						//exibe a lista de tags (tag_id user_id sofifa_id tag)

void save_as_trie();

//VARIAVEIS GLOBAIS
s_players lista_jogadores[QJ];	//array de jogadores structs
s_ratings lista_ratings[QR];  	//array de ratings
s_tags lista_tags[QT];			//array de tags

Trie* tree;

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
int main()
{
	//le os jogadores do CSV e exibe a lista de estruturas salvas

	read_players_csv();
	//print_players(20);

	save_as_trie();

	cout << "######################" << endl;

	//read_tags_csv();
	//print_tags();

	cout << "######################" << endl;

	//read_ratings_csv();
	//print_ratings();


	return 0;
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//##########################################
//##########    I/O PLAYERS   ##############
//##########################################
//le o arquivo players.csv e salva no vetor de structs
void read_players_csv()
{
    ifstream inFile(PLAYERSFILE, ios_base::in);

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

		//POSICAO
		getline(linestream, item, ',');//tenta ler posicao

        if(linenum > 0){
            //verifica se existem ASPAS. se sim, significa que o array das posicoes tem mais de 1 posicao
            if(item.front() == '"'){

                item.erase(item.begin());	//remove as aspas da primeira posicao
                pp.push_back(item);		 //insere primeira posicao no vetor


                //vai ler até a proxima ", ou seja, vai todas as posicoes, então vamos ter que tratar essa string depois
                getline(linestream, item, '"');

                char handle;
                int index=0, lastIndex=0;
                int tam = item.length();
                string posicao;

                do{
                    handle = item[index];
                    if (handle==' '){
                        item.erase(item.begin());
                    }
                    if(handle==',' || index==tam-1){

                        posicao = item.substr(lastIndex,index);
                        lastIndex = index;
                        pp.push_back(posicao);
                        item.erase(lastIndex,index);
                        tam = item.length();

                    }
                    index++;
                }while(index<tam);
            }
            else{// //caso contrario, so existe 1 posicao.
                pp.push_back(item);
            }

		jogador.player_positions = pp;

		//add the new jogador data to the database
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		//if(linenum > 0){
			lista_jogadores[linenum-1] = jogador;
		}

		linenum++;
	}
}

void save_as_trie(){
    tree=new Trie();
    for (int x=0;x<QJ;x++){
        tree->insert(lista_jogadores[x].name);
    }

}

void print_players(int qtd){
	cout 	<< left << setw(7)  << "N"
			<< left << setw(50) << "NOME"
			<< left << setw(15) << "ID"
			<< left << setw(15) << "POSICOES"
			<< endl;

	//output the jogadores data.
	for(int i = 0; i < qtd; i++)
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
		cout << pp[j]<< " ";
	}
}


//##########################################
//##########    I/O RATINGS	  ##############
//##########################################
void read_ratings_csv()
{
    ifstream inFile(RATINGSFILE); //our file
    string line;
    int linenum = 0;

    while (getline (inFile, line))
    {
		s_ratings r; //temp rating struct for use in the while loop

        stringstream linestream(line);
        string item;

		//USER_ID
		getline(linestream, item, ',');
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			stringstream ss(item);
			ss >> r.user_id;
		}

		//SOFIFA_ID
		getline(linestream, item, ',');	//convert to a string stream
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			stringstream ss(item);
			ss >> r.sofifa_id;

		}

		// //verifica se existem ASPAS. se sim, significa que o array das posicoes tem mais de 1 posicao
		getline(linestream, item, ',');
		if(linenum > 0){
			stringstream ss(item);
			ss >> r.rating;
		}

		//add the new jogador data to the database
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			lista_ratings[linenum-1] = r;
		}

		linenum++;
	}
}

void print_ratings(){
	cout 	<< left << setw(10) << "USER_ID"
			<< left << setw(10) << "SOFIFA_ID"
			<< left << setw(10) << "RATING"
			<< endl;

	//output the ratings data.
	for(int i = 0; i < QR; i++)
	{
		cout 	<< left << setw(10) << lista_ratings[i].user_id
				<< left << setw(10) << lista_ratings[i].sofifa_id
				<< left << setw(10) << lista_ratings[i].rating
				<< endl;
	}
}

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

		//t.tag_id = linenum;

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

	//output the tags data.
	for(int i = 0; i < QT; i++)
	{
		cout
		//<< left << setw(10) << lista_tags[i].tag_id
				<< left << setw(10) << lista_tags[i].user_id
				<< left << setw(10) << lista_tags[i].sofifa_id
				<< left << setw(50) << lista_tags[i].tag
				<< endl;
	}
}
