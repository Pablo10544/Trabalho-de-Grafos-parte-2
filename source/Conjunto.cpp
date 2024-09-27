#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include "../include/Graph.hpp"
#include "../include/Node.hpp"
#include "../include/Edge.hpp"
#include "../include/Conjunto.hpp"

using namespace std;


Conjunto::Conjunto(size_t n){
    pai.resize(n+1);
    altura.resize(n+1,0);
    for (size_t i = 1; i <= n; i++) {
        pai[i] = i;
    }
}

size_t Conjunto::find(size_t u) {
    if (pai[u] != u) {
        pai[u] = find(pai[u]);
    }
    return pai[u];
}

void Conjunto::unite(size_t u, size_t v) {
    size_t rootU = find(u);
    size_t rootV = find(v);

    if (rootU != rootV) {
        
        if (altura[rootU] > altura[rootV]) {
            pai[rootV] = rootU;
        } else if (altura[rootU] < altura[rootV]) {
            pai[rootU] = rootV;
        } else {
            pai[rootV] = rootU;
            altura[rootU]++;
        }
    }
}
vector<size_t> Conjunto::getpai(){
    return this->pai;
}