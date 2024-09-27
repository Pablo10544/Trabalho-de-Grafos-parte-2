#ifndef GRAFO_BASICO_EDGE_H
#define GRAFO_BASICO_EDGE_H

#include "defines.hpp"

struct Edge
{
    Edge  *_next_edge;
    float  _weight;
    size_t _source_id;
    size_t _target_id;
    bool _gemea; // indica que a aresta ja existe no sentido contrario (usada em grafos nao direcionados)
};

#endif /* GRAFO_BASICO_EDGE_H */
