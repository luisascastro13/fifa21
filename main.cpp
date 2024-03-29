#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <list> 

///Users/ldecastro/Desktop/pessoal/UFRGS/3osem/CPD/projetofinal/fifa21-main/main.cpp
//CONSTANTES
#define QJ 18944	//QUANTIDADE DE JOGADORES 	- tabela original: 18944
#define QR 10000//24188078	//QUANTIDADE DE RATINGS 	- tabela original: 24188078       MINIRATING: 
//#define QR 24188078 
#define QT 364950 	//QUANTIDADE DE TAGS		- tabela original: 364950
#define PRIMO 22739
#define COUNT_MAIOR 2//quantidade de avalaiaçõ pra pesquisa 3

//nome dos arquivos
#define PLAYERSFILE "base/players.csv"
//#define RATINGSFILE "base/minirating.csv"
#define RATINGSFILE "rating.csv"
#define TAGSFILE "base/tags.csv"

//ESTRUTURAS
#include "s_players.h"
#include "s_ratings.h"
#include "s_tags.h"

#include "trie.cpp"
#include "hash.cpp"
#include "split.cpp"

using namespace std;

// PROTOTIPOS DE FUNCOES
void read_players_csv(); 				//le o arquivo players.csv e salva no vetor de structs
void print_cabecalho();
void print_player(s_players player);
void print_players(vector<s_players> lista); //exibe uma lista dos jogadores (nome id)
void print_playerpos(s_players jog); 	//exibe a lista de posicoes do jogador jog
void read_ratings_csv();
void print_ratings();
void read_tags_csv();					//le arquivo tags.csv e salva no vetor de structs
void print_tags();						//exibe a lista de tags (tag_id user_id sofifa_id tag)
void print_playertags(s_players player);

void save_as_trie();
void menu();

void print_busca2(vector<s_ratings> res);
void print_busca3(vector <s_players> lista, int l);
void print_busca4(vector <s_players> lista);

//VARIAVEIS GLOBAIS
vector<s_players> lista_jogadores;//[QJ];	//array de jogadores structs
vector <s_ratings> lista_ratings;//[QR];  	//array de ratings
vector<s_tags> lista_tags;//[QT];			//array de tags

struct TrieNode *root;
Hash table(PRIMO);

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
int main()
{
	clock_t t;
	//le os jogadores do CSV e salva na arvore trie
	t = clock();
	read_players_csv();	
	save_as_trie();

	//table(QJ); salva na tabela hash
	for (int i=0;i<lista_jogadores.size();i++){
		table.insertItem(lista_jogadores[i]);	
	}
	//le ratings e tags e atualiza os valores

	read_ratings_csv();
	read_tags_csv();
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
		cout << "\nDigite um comando para pesquisa ou -1 para sair: \n";
		std::getline(std::cin, comando);

		if (!comando.substr(0,6).compare("player")){//comando player id
			busca = comando.substr(7,comando.length()-7);
			vector<int> res= search(root, busca);

			s_players player;
			print_cabecalho();
			for (int x=0;x<res.size();x++){
				player = table.searchItem(res[x]);
				print_player(player);
				print_playerpos(player);
			}
		}
		else if (!comando.substr(0,4).compare("user")){//comando user id
			busca = comando.substr(5,comando.length()-5);
			vector<s_ratings> res = getUserRatings(lista_ratings,stoi(busca));
			ordenaRatings(res);
			print_busca2(res);//PRINT BUSCA COM MAX 20
		}
		else if (!comando.substr(0,3).compare("top")){
			string top = comando.substr(3, comando.length()-3);
			split(comando, ' ');
			//TOP N
			string topN = strings[0];
			topN.erase(topN.begin());
			topN.erase(topN.begin());
			topN.erase(topN.begin()); //remove a palavra top

			//POSICAO
			string posicao = strings[1];
			posicao.erase(posicao.begin());
			posicao.erase(posicao.size()-1); //remove as aspas da posicao

			//procurar jogadores que tenham essa posicao E count > 1000
			vector<s_players> p;
			vector<s_players> tabela;

			tabela = table.returnList(); //toda a lista de jogadores com seus counts
			p = getPlayersMaiorMil(tabela,COUNT_MAIOR); //procura jogadores que tem count > 1000
			p = getPlayerWithPosition(p, posicao); //procura jogadores que tem a posicao
			ordenaPlayersPorRating(p); //ordena pelo rating
			print_busca3(p,stoi(topN)); //imprime somente o topN necessario			
		}
		else if(!comando.substr(0,4).compare("tags")){
			string top = comando.substr(4, comando.length()-4);

			split(comando, ' ');

			//salva as tags em um vetor
			vector<string> tagsPesquisa;
			for(int x=1; x<max;x++){
				if(strings[x]!= ""){
					string tag = strings[x];
					tag.erase(tag.begin());
					tag.erase(tag.end() - 1); //remove as aspas da tag
					
					tagsPesquisa.push_back(tag);
				}
			}
						
			vector<s_players> tabela = table.returnList(); //toda a lista de jogadores

			vector<s_players> pTags = playersWithAllTags(tabela, tagsPesquisa);

			print_busca4(pTags);

				
		}		
		else if (comando.compare("-1")==0){
			break;
		}
		else{
			cout << "\nHouve um erro de digitação... Digite novamente: ";
		}

	}while(comando.compare("-1")!=0);
}
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


//##########################################
//##########   PRINT BUSCAS   ##############
//##########################################

void print_busca2(vector<s_ratings> res){
	cout 	<< left << setw(15) << "\nSOFIFA_ID"
			<< left << setw(50) << "NAME"	
			<< left << setw(10) << "G_RATING"
			<< left << setw(10) << "COUNT"		
			<< left << setw(20) << "U_RATING"
			<< endl;
	s_players player;

	for(int i=0; i<res.size() && i<20;i++){
		player = table.searchItem(res[i].sofifa_id);
		cout	<< left << setw(15) << player.sofifa_id
				<< left << setw(50) << player.name
				<< left << setw(10) << player.rating
				<< left << setw(10) << player.count
				<< left << setw(10) << res[i].rating
				<<endl;
	}
	
}

void print_busca3(vector <s_players> lista, int l){
	print_cabecalho();
	//output the jogadores data.
	for(int i = 0; i < lista.size() && i < l; i++)
	{
		print_player(lista[i]);		
		print_playerpos(lista[i]);
		cout << endl;
	}
}

void print_busca4(vector <s_players> lista){
	print_cabecalho();
	//output the jogadores data.
	for(int i = 0; i < lista.size(); i++)
	{
		print_player(lista[i]);		
		print_playerpos(lista[i]);
		cout << endl;
	}
}
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

		jogador.count=0;
		jogador.rating=0;

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
	}

}

void print_players(vector <s_players> lista){
	print_cabecalho();
	//output the jogadores data.
	for(int i = 0; i < lista.size(); i++)
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

void print_playertags(s_players player){
	vector<string> pp = player.player_tags;
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
				
			table.updateCount(table.searchItem(r.sofifa_id).sofifa_id);
			table.updateRating(table.searchItem(r.sofifa_id).sofifa_id, r.rating);
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

		//TAGS
		getline(linestream, item, ',');
		t.tag = item;

		table.updateTag(t.sofifa_id, t.tag);

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