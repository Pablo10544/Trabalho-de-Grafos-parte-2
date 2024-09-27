#ifndef MGGPPAUX_HPP
#define MGGPPAUX_HPP

#include "Edge.hpp"
#include <vector>
#include "defines.hpp"

using namespace std;

struct MGGPPAux
{
    vector<Edge*> edges;
    size_t minvalue;
    size_t maxvalue;
    size_t gap;
};
#endif /* GRAFO_BASICO_EDGE_H */
