#define infinito 99999//65535
#include "include/Graph.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int menu() {
    int selecao;
    bool again;
    do{
        again = false;
        cout << endl;
        cout << "   ------  MENU ------" << endl;
        cout << "[1] Fecho transitivo direto de um vertice" << endl; // ok 
        cout << "[2] Fecho transitivo indireto de um vertice" << endl;  // ok
        cout << "[3] Caminho minimo entre dois vertices - Dijkstra" << endl; // ok
        cout << "[4] Caminho minimo entre dois vertices - Floyd" << endl; // ok
        cout << "[5] Arvore Geradora Mínima de subgrafo vertice-induzido - Prim" << endl; // ok
        cout << "[6] Arvore Geradora Mínima de subgrafo vertice-induzido - Kruskal" << endl; // ok 
        cout << "[7] Arvore dada pela ordem do caminhamento em profundidade a partir de um vertice" << endl; // ok
        cout << "[8] Raio, centro, diametro e periferia do grafo" << endl; // ok
        cout << "[9] Conjunto de vertices de articulacao" << endl; // ok
	cout << "[99] Salvar lista de adjacência do grafo no arquivo de saída" << endl; 
        cout << "[0] Sair" << endl;
        cout << "Escolha: ";
        cin >> selecao;
    }while(again);
        if(!cin){
            again = true;
            cin.clear();
            cin.ignore();
        }
        return selecao;
    }


int main(int argc, char* argv[]){    
    // Lê o arquivo.
    string nomeArquivoEntrada = argv[1];
    cout << "Criando instancia do arquivo\n";
    ifstream arquivo_entrada(nomeArquivoEntrada);
    // Confere se o arquivo existe
    if (!arquivo_entrada){ 
        cout << "Arquivo não encontrado.\n" ;
        return 0;
    }
    
    Graph *grafo = new Graph(arquivo_entrada, false, false, true);
    

    arquivo_entrada.close();
    grafo->~Graph();
    
    return 0;
}

