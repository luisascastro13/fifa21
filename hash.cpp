#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "s_players.h"
using namespace std;


 
class Hash
{
    int BUCKET;    
 
    // Pointer to an array containing buckets
    list<s_players> *table;
public:
    Hash(int V);  
 
    // inserts a key into hash table
    void insertItem(s_players player);
  
    // hash function 
    int hashFunction(int x) {
        return (x % BUCKET);
    }

    //search key na tabela
    s_players searchItem(int key);
 
    void displayHash();
    void updateCount(int key);
    void updateRating(int key, float rat);
    vector<s_players> returnList();
    void updateTag(int key, string tag);
};
 
Hash::Hash(int b)
{
    this->BUCKET = b;
    table = new list<s_players>[BUCKET];
}
 
void Hash::insertItem(s_players player)
{
    int index = hashFunction(player.sofifa_id);
    table[index].push_back(player);
}

s_players Hash::searchItem(int key)//sofifa_id
{
  // get the hash index of key
  int index = hashFunction(key);
 
  // encontra key na (index)th list
  list <s_players> :: iterator i;
  for (i = table[index].begin();i != table[index].end(); i++) {
    if ((*i).sofifa_id == key)
      break;
  }
 
  // se encontrou, retorna
  return (*i);
}

vector<s_players> Hash::returnList(){
  vector<s_players> res;
  int index = hashFunction(PRIMO);
  list <s_players> :: iterator i;
  for(int a=0; a < PRIMO; a++){
    int index = hashFunction(a);
    for (i = table[index].begin();i != table[index].end(); i++) {
      s_players p = (*i);
      res.push_back(p);
    }
  }
  
  return res;
}
 
 // function to display hash table
void Hash::displayHash() {
  for (int i = 0; i < BUCKET; i++) {
    cout << i;
    for (auto x : table[i]){
      //cout << " --> " << x;
    }
    cout << endl;
  }
}

void Hash::updateCount(int key){
  // get the hash index of key
  int index = hashFunction(key);
 
  // find the key in (index)th list
  list <s_players> :: iterator i;
  for (i = table[index].begin();i != table[index].end(); i++) {
    if ((*i).sofifa_id == key){
      (*i).count++;    
      break;
    }      
  }
}

void Hash::updateRating(int key, float rat){
  // get the hash index of key
  int index = hashFunction(key);
 
  // find the key in (index)th list
  list <s_players> :: iterator i;
  for (i = table[index].begin();i != table[index].end(); i++) {
    if ((*i).sofifa_id == key){
      float soma = ((*i).rating * ((*i).count -1))+rat;
      (*i).rating =  soma/(*i).count;
      break;
    }      
  }
}

void Hash::updateTag(int key, string tag){
  // get the hash index of key
  int index = hashFunction(key);
 
  // find the key in (index)th list
  list <s_players> :: iterator i;
  for (i = table[index].begin();i != table[index].end(); i++) {
    if ((*i).sofifa_id == key){
      (*i).player_tags.push_back(tag);  
      break;
    }      
  }
}