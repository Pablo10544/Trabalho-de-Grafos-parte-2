#define infinito 99999//65535
#include "include/Graph.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){    
    // Lê o arquivo.
    if (argc != 3){
        cout << "Uso incorreto. Siga o padrão: ./graph_project <arquivo do grafo> <1, 2 ou 3 (1 para algoritmo GULOSO, 2 para RANDOMIZADO e 3 para REATIVO)>\n";
        return 0;
    }
    
    string nomeArquivoEntrada = argv[1];
    // cout << "Criando instancia do arquivo\n";
    ifstream arquivo_entrada(nomeArquivoEntrada);
    // Confere se o arquivo existe
    if (!arquivo_entrada){ 
        cout << "Arquivo não encontrado.\n" ;
        return 0;
    }
    string optstr = argv[2];
    //string alfar = argv[3];
    //float al = stof(alfar);
    int opt = stoi(optstr);
    
    if (opt > 3 || opt < 1){
        cout << "Uso incorreto. O ultimo parametro deve estar entre 1 e 3\n";
        return 0;
    }

    Graph *grafo = new Graph(arquivo_entrada, false, false, true, opt,0.2);
    

    arquivo_entrada.close();
    grafo->~Graph();
    
    return 0;
}

