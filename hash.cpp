#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include<bits/stdc++.h>

#include "s_players.h"
using namespace std;

#define PRIMO 22739//?
 
class Hash
{
    int BUCKET;    // No. of buckets
 
    // Pointer to an array containing buckets
    list<s_players> *table;
public:
    Hash(int V);  // Constructor
 
    // inserts a key into hash table
    void insertItem(s_players player);
  
    // hash function to map values to key
    int hashFunction(int x) {
        return (x % BUCKET);
    }

    //search for a key in th e hash table
    s_players searchItem(int key);
 
    void displayHash();
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
 
  // find the key in (index)th list
  list <s_players> :: iterator i;
  for (i = table[index].begin();i != table[index].end(); i++) {
    if ((*i).sofifa_id == key)
      break;
  }
 
  // if key is found in hash table, returns it
    return (*i);

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
