#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


using namespace std;
 
const int ALPHABET_SIZE = 32;
 
// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
     
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
    int id;
};
 
// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode =  new TrieNode;
 
    pNode->isEndOfWord = false;
 
    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;
 
    return pNode;
}
 

int getCharIndex(char caracter){
    if (caracter>='A' && caracter <='z'){
        caracter=tolower(caracter);
        return caracter-'a';
    }
    else if (caracter==' '){
        return 27;
    }else if(caracter=='-'){
        return 28;
    }else if(caracter=='.'){
        return 29;
    }else if (caracter=='\''){
        return 30;
    }else if(caracter=='"'){
        return 31;
    }    
    else{
        return 32;
    }
     
}
// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(struct TrieNode *root, s_players player)
{
    struct TrieNode *pCrawl = root;
    string key = player.name;
 
    for (int i = 0; i < key.length(); i++)
    {
       
        int index = getCharIndex(key[i]);
        if (!pCrawl->children[index]){
            pCrawl->children[index] = getNode();
        }
 
        pCrawl = pCrawl->children[index];
    }
 
    // mark last node as leaf
    pCrawl->isEndOfWord = true;
    //save player id 
    pCrawl->id=player.sofifa_id;
}

void depth_search(struct TrieNode *root, vector<int>& idList){
    struct TrieNode *pCrawl = root;
    
    bool empty=true;

    if (pCrawl->isEndOfWord){
        idList.push_back(pCrawl->id);
    }
    else{
        for (int x=0;x<ALPHABET_SIZE;x++){
            if (pCrawl->children[x]!=NULL){//se algum nodo tiver conteudo chama a funcao com ele
                 depth_search(pCrawl->children[x], idList);
            }
        }
    }
}

// Returns vector of players' ids found
vector<int> search(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;
    vector<int> idList;
    for (int i = 0; i < key.length(); i++)
    {
        
        
        int index = getCharIndex(key[i]);

        if (!pCrawl->children[index]){//não tem nodo
            return idList;
        }
 
        pCrawl = pCrawl->children[index];
    }
    if (pCrawl != NULL){ // começa  a fazer a pesquisa de todos os nomes que contem a key
        depth_search(pCrawl, idList);
    }
    return idList;
}