#ifndef CONJUNTO_HPP
#define CONJUNTO_HPP

#include <iostream>
#include <vector>
#include <set>
#include "Node.hpp"
#include "defines.hpp"

using namespace std;

class Conjunto {
public:
    Conjunto(size_t n);
    size_t find(size_t u);
    void unite(size_t u, size_t v);
    vector<size_t> getpai();
private:
    vector<size_t> pai;
    vector<size_t> altura;
};

#endif