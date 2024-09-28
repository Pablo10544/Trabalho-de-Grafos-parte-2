#ifndef CANDIDATO_HPP
#define CANDIDATO_HPP

#include "Edge.hpp"
using namespace std;

class Candidato {
    public:
    int posicao;
    int custoAdicional;
    Edge* arestaCandidato;
};

#endif