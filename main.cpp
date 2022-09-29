#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>

//CONSTANTES
#define QJ 18944	//QUANTIDADE DE JOGADORES 	- tabela original: 18944
#define QR 10000//24188078	//QUANTIDADE DE RATINGS 	- tabela original: 24188078       MINIRATING: 
#define QT 364950 	//QUANTIDADE DE TAGS		- tabela original: 364950

//nome dos arquivos
#define PLAYERSFILE "players.csv"
#define RATINGSFILE "minirating.csv"//"rating.csv"
#define TAGSFILE "tags.csv"

//ESTRUTURAS
#include "s_players.h"
#include "s_ratings.h"
#include "s_tags.h"

#include "trie.cpp"
#include "hash.cpp"

using namespace std;

// PROTOTIPOS DE FUNCOES
void read_players_csv(); 				//le o arquivo players.csv e salva no vetor de structs
void print_cabecalho();
void print_player(s_players player);
void print_players(int qtd, vector<s_players> lista); 					//exibe a lista dos jogadores (nome id)
void print_playerpos(s_players jog); 	//exibe a lista de posicoes do jogador jog
void read_ratings_csv();
void print_ratings();

void read_tags_csv();					//le arquivo tags.csv e salva no vetor de structs
void print_tags();						//exibe a lista de tags (tag_id user_id sofifa_id tag)

void save_as_trie();
void menu();

//VARIAVEIS GLOBAIS
vector<s_players> lista_jogadores;//[QJ];	//array de jogadores structs
vector <s_ratings> lista_ratings;//[QR];  	//array de ratings
vector<s_tags> lista_tags;//[QT];			//array de tags

struct TrieNode *root;
Hash table(QJ);

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
int main()
{
	clock_t t;
	//le os jogadores do CSV e exibe a lista de estruturas salvas
	t = clock();
	read_players_csv();
	save_as_trie();
	//read_tags_csv();
	//read_ratings_csv();

	//table(QJ);
	for (int i=0;i<lista_jogadores.size();i++){
		table.insertItem(lista_jogadores[i]);	
	}
	

	t = clock() - t;

	cout << "Time: " << ((double)t)/(CLOCKS_PER_SEC);

	cout << "\n######################" << endl;

	//print_players(20);
	
	cout << "######################" << endl;
	
	//print_tags();

	cout << "######################" << endl;
	
	//print_ratings();

	cout << "######################" << endl;

	menu();

	return 0;
}
void menu (){
	string comando, busca;
	do{
		cout << "Digite um comando para pesquisa ou -1 para sair: \n";
		std::getline(std::cin, comando);

		if (!comando.substr(0,6).compare("player")){
			busca = comando.substr(7,comando.length()-7);
			vector<int> res= search(root, busca);

			s_players player;
			print_cabecalho();
			for (int x=0;x<res.size();x++){
				player = table.searchItem(res[x]);
				print_player(player);
				print_playerpos(player);
			}
			//print lista aqui
		}
		else if (!comando.substr(0,4).compare("user")){
			vector<s_ratings> res = getUserRatings(lista_ratings,stoi(busca));//,108952);
			ordenaRatings(res);
			//print lista com max 20 aqui
		}
		//else if (comando.substring())...
		//else if...
		/*else if (comando.compare("-1")==0){
			break;
		}
		else{
			cout << "\nHouve um erro de digitação... Digite novamente: ";
		}*/

	}while(comando.compare("-1")!=0);
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


                //vai ler at� a proxima ", ou seja, vai todas as posicoes, ent�o vamos ter que tratar essa string depois
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
			//lista_jogadores[linenum-1] = jogador;
			lista_jogadores.push_back(jogador);
		}

		linenum++;
	}
}

void save_as_trie(){
	root = getNode();
    for (int x=0;x<QJ;x++){
		insert(root, lista_jogadores[x]);
		//cout << "\n" << lista_jogadores[x].name;
	}

}

void print_players(int qtd, vector <s_players> lista){
	
	//output the jogadores data.
	for(int i = 0; i < qtd; i++)
	{
		print_player(lista[i]);		
		print_playerpos(lista[i]);
		cout << endl;
	}
}

void print_cabecalho(){
	cout 	<< left << setw(15) << "\nSOFIFA_ID"
			<< left << setw(50) << "NAME"	
			<< left << setw(10) << "RATING"
			<< left << setw(10) << "COUNT"		
			<< left << setw(20) << "POSITIONS"
			<< endl;
}

void print_player(s_players player){
	cout	<< left << setw(15) << player.sofifa_id
			<< left << setw(50) << player.name
			<< left << setw(10)  << player.rating
			<< left << setw(10)  << player.count;
}

void print_playerpos(s_players jog){
	vector<string> pp = jog.player_positions;
	//output the players positions.
	for(int j =0 ; j < pp.size();j++){
		cout << pp[j]<< " ";
	}
	cout << endl;
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

		//add the new rating data to the database
		//se nao for a primeira linha do CSV (pq eh o nome das colunas)
		if(linenum > 0){
			//lista_ratings[linenum-1] = r;
			lista_ratings.push_back(r);
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
			//lista_tags[linenum-1] = t;
			lista_tags.push_back(t);
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
