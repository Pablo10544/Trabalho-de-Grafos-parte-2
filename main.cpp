#define infinito 99999//65535
#include "include/Graph.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
    cout << "INICIO!"<< endl;
    cout << "a";
    if (argc != 3){
        cout << "Uso errado. Use ./<executável> <arquivo do grafo> <1, 2 ou 3 (1 para GULOSO, 2 para RANDOMIZADO ou 3 para REATIVO)>\n";
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
    string optstring = argv[2];
    int opt = stoi(optstring);

    if (opt < 1 || opt > 3){
        cout << "Terceiro argumento inválido. Precisa ser um número entre 1 e 3\n1 para GULOSO, 2 para RANDOMIZADO ou 3 para REATIVO\n";
	return 0;
    }

    
    Graph *grafo = new Graph(arquivo_entrada, false, false, true, opt);
    

    arquivo_entrada.close();
    grafo->~Graph();
    
    return 0;
}

