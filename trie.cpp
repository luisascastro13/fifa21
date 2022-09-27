#include <iostream>
#include <string>
using namespace std;

// Define o tamanho do caractere
#define CHAR_SIZE 128

// Uma classe para armazenar um n� Trie
class Trie
{
public:
    bool isLeaf;
    Trie* character[CHAR_SIZE];

    // Construtor
    Trie()
    {
        this->isLeaf = false;

        for (int i = 0; i < CHAR_SIZE; i++) {
            this->character[i] = nullptr;
        }
    }

    void insert(string);
    bool deletion(Trie*&, string);
    bool search(string);
    bool haveChildren(Trie const*);
};

// Fun��o iterativa para inserir uma chave em um Trie
void Trie::insert(string key)
{
    //cout << "key "<< key;
    // inicia do n� raiz
    Trie* curr = this;
    for (int i = 0; i < key.length(); i++)
    {
        // cria um novo n� se o caminho n�o existir
        if (curr->character[key[i]] == nullptr) {
            curr->character[key[i]] = new Trie();
        }

        // vai para o pr�ximo n�
        curr = curr->character[key[i]];
    }

    // marca o nodo atual como uma folha
    curr->isLeaf = true;
}

// Fun��o iterativa para pesquisar uma chave em um Trie. Ele retorna verdadeiro
// se a chave for encontrada no Trie; caso contr�rio, retorna falso
bool Trie::search(string key)
{
    // retorna false se Trie estiver vazio
    if (this == nullptr) {
        return false;
    }

    Trie* curr = this;
    for (int i = 0; i < key.length(); i++)
    {
        // vai para o pr�ximo n�
        curr = curr->character[key[i]];

        // se a string for inv�lida (alcan�ou o final de um caminho no Trie)
        if (curr == nullptr) {
            return false;
        }
    }

    // retorna true se o n� atual for uma folha e o
    // fim da string � atingido
    return curr->isLeaf;
}

// Retorna true se um determinado n� tiver algum filho
bool Trie::haveChildren(Trie const* curr)
{
    for (int i = 0; i < CHAR_SIZE; i++)
    {
        if (curr->character[i]) {
            return true;    // filho encontrado
        }
    }

    return false;
}

// Fun��o recursivo para deletar uma chave no Trie
bool Trie::deletion(Trie*& curr, string key)
{
    // retorna se Trie estiver vazio
    if (curr == nullptr) {
        return false;
    }

    // se o final da chave n�o for alcan�ado
    if (key.length())
    {
        //recorre para o n� correspondente ao pr�ximo caractere na chave
        // e se retornar verdadeiro, exclui o n� atual (se n�o for folha)

        if (curr != nullptr &&
            curr->character[key[0]] != nullptr &&
            deletion(curr->character[key[0]], key.substr(1)) &&
            curr->isLeaf == false)
        {
            if (!haveChildren(curr))
            {
                delete curr;
                curr = nullptr;
                return true;
            }
            else {
                return false;
            }
        }
    }

    // se o fim da chave for atingido
    if (key.length() == 0 && curr->isLeaf)
    {
        // se o nodo atual for um nodo folha e n�o tiver filhos
        if (!haveChildren(curr))
        {
            //deleta o n� atual
            delete curr;
            curr = nullptr;

            // exclui os n�s pais n�o-folha
            return true;
        }

        // se o nodo atual for um nodo folha e tiver filhos
        else {
            // marca o n� atual como um n� n�o folha (DON'T DELETE IT)
            curr->isLeaf = false;

            // n�o exclui seus n�s pais
            return false;
        }
    }

    return false;
}
