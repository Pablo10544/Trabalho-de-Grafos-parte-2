#define infinito 99999//65535
#include "../include/Graph.hpp"
#include "../include/Node.hpp"
#include "../include/Edge.hpp"
#include "../include/Conjunto.hpp"
#include "../include/MGGPPAux.hpp"
#include <iostream>
#include <cstddef>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <math.h>
#include <chrono>
#include <ctime> 

using namespace std;
Graph::Graph(ifstream& instance, bool direcionado, bool weighted_edges, bool weighted_nodes){
    // Pega a primeira linha e joga pra tamanhoInstância (a 1° linha é o tamanho da instancia do grafo, check README.txt)
    // cout << "Inicio do construtor\n";
    string name; // temporario pra ser usado na função getline()
    getline(instance, name);
    cout<<"name"<<name;
    // pega o tamanho da instancia em inteiro
   // this->_number_of_nodes = stoi(temp); // stoi = string to int
    
    //adj.resize(this->_number_of_nodes+1);
    this->_directed = direcionado;
    this->_weighted_edges = weighted_edges;
    this->_weighted_nodes = weighted_nodes;

    // ANALISE DE ARQUIVO DE ENTRADA
    _first=nullptr;
    string linha;
    string vertices;
    string pesosVertices;
    string arestasConectados;
    this->_number_of_edges = 0;
    this->_number_of_nodes = 0;
    bool parametroNumeroDeVerticesEncontrados=false;
    bool parametroPesosVertices=false;
    bool parametroArestaConectado = false;

    cout << "Começando a ler arquivo.\n";
    while (getline(instance, linha)){ // le cada linha
        stringstream ss(linha);
        if(linha.find("param p :=")!=string::npos){
            size_t posicaoParametro = linha.find("=")+1;
            size_t tamanho = linha.find(';')-posicaoParametro;
           this->numeroSubGrafos = stoi(linha.substr(posicaoParametro,tamanho));
        }
        else if(linha.find("set V := ")!=string::npos)
        {
            parametroNumeroDeVerticesEncontrados = true;
            continue;
        }
        else if(linha.find("param w := ")!=string::npos)
        {
            parametroPesosVertices=true;
            continue;
        }
        else if(linha.find("set E := ")!=string::npos)
        {
            parametroArestaConectado = true;
            continue;
        }
        if(parametroNumeroDeVerticesEncontrados)
        {
            if(linha.find(";")!=string::npos){
                parametroNumeroDeVerticesEncontrados = false;
            }
            else
            {
                vertices+=linha;
            }
        }
        if(parametroPesosVertices){
            if(linha.find(";")!=string::npos){
                parametroPesosVertices = false;
            }else{
                cout<<"vertice:"<<linha.substr(0,3)<<" de peso: "<<linha.substr(5,3)<<endl;
                add_node(stoi(linha.substr(0,3)),stoi(linha.substr(5,3)));
                pesosVertices+=linha+"\n";
            }
        }
        if(parametroArestaConectado){
            if(linha.find(";")!=string::npos){
                parametroArestaConectado = false;
            }else{
                string primeiroVertice="";
                size_t idPrimeiroNo;
                string segundoVertice="";
                size_t idSegundoNo;
                bool inicioPrimeiroCaracter=false;
                bool inicioSegundoCaracter = false;
                arestasConectados +=linha;
                  for (char i : linha){
                    if (i == '('){
                        inicioPrimeiroCaracter = true;
                        continue;
                    }
                    else if(i==','){
                        inicioPrimeiroCaracter = false;
                        idPrimeiroNo = stoi(primeiroVertice);
                        primeiroVertice="";
                        inicioSegundoCaracter = true;
                      //  cout<<"Primeiro caracter"<<idPrimeiroNo;
                        continue;
                    }else if(i==')'){
                        inicioSegundoCaracter = false;
                        idSegundoNo = stoi(segundoVertice);
                        segundoVertice = "";
                       // cout<<"Segundo caracter"<<idSegundoNo<<endl;
                        add_edge(idPrimeiroNo,idSegundoNo);
                  //  cout<<"<edge source=\""<<idPrimeiroNo<<"\""<<" "<<"target=\""<<idSegundoNo<<"\"/>"<<endl;
                        add_edge(idSegundoNo,idPrimeiroNo);
                        continue;
                    }
                    if(inicioPrimeiroCaracter){
                        primeiroVertice+=i;
                    }
                    if(inicioSegundoCaracter){
                        segundoVertice+=i;
                    }
            }
            }
        }
    }
    cout<<"Numero de subgrafos: "<<numeroSubGrafos<<endl;
    cout<<"vertices"<<vertices<<endl;
   // cout<<"Pesos dos vertices"<<pesosVertices<<endl;
   // cout<<"Arestas Conectadas"<<arestasConectados<<endl;
    cout << "Terminou de ler \n";
    this->raio = infinito;
    this->diametro = 0;
   // print_graph();
    vector<Edge*> vetor = AlgoritmoGuloso();

}

// usado para ordenação
bool Graph::compararArestas(const Edge* a, const Edge* b) { // compara duas arestas e retorna verdadeiro se A tem menor gap que B
    int diffA = abs(search_for_node(a->_source_id)->_weight - search_for_node(a->_target_id)->_weight);// pesoVertice(a.u) - pesoVertice(a.v);
    int diffB = abs(search_for_node(b->_source_id)->_weight - search_for_node(b->_target_id)->_weight);;
    return diffA < diffB;
}

vector<Edge*> Graph::order_edges_non_decreasing_gap(){
    vector<Edge*> retorno;
      Node* traversal=this->_first;
    Edge* edgeTraversal = traversal->_first_edge;
    while(traversal!=nullptr){
        edgeTraversal = traversal->_first_edge;
        while(edgeTraversal!=nullptr){
                        retorno.push_back(edgeTraversal);
            
           edgeTraversal=edgeTraversal->_next_edge;
        }
        traversal=traversal->_next_node;
    }
   // cout<<"tamanho retorno: "<<retorno.size();
   std::sort(retorno.begin(), retorno.end(),
                  [this](const Edge* e1, const Edge* e2) {
                      return compararArestas(e1, e2);  // Chama a função membro
                  }); 
                //   for(int i=0;i<retorno.size();i++){
                //    cout<<"aresta"<<retorno[i]->_source_id<<"-"<<retorno[i]->_target_id<<"custo:"<<search_for_node(retorno[i]->_source_id)->_weight- search_for_node(retorno[i]->_target_id)->_weight<<endl;
                //   }   
                  return retorno;
}

int Graph::determinar_gap_aresta(Edge* aresta){
    Node* source = search_for_node(aresta->_source_id);
    Node* target = search_for_node(aresta->_target_id);
    return abs(source->_weight - target->_weight);
}

// busca se determinada aresta é adj a outra aresta num conjunto
bool Graph::arestas_adj(vector<Edge*> arestas, Edge* alvo){
    for (auto aresta : arestas){
        if(aresta->_source_id == alvo->_source_id || aresta->_target_id == alvo->_target_id
            || aresta->_target_id == alvo->_source_id || aresta->_source_id == alvo->_target_id){
            return true;
        } 
    }
    return false;
}
vector<Edge*> Graph::getCutEdges(vector<Edge*> _MGGPPAuxEdges,vector<Edge*> visitedEdges) {
    std::vector<Edge*> cutEdges;
 
    for (const auto& edge : _MGGPPAuxEdges) {
        Node* source = search_for_node(edge->_source_id);
        Edge* sourceEdge = source->_first_edge;
        while(sourceEdge!=nullptr){
            if(!aresta_no_vetorIdaVolta(cutEdges,sourceEdge)&&!aresta_no_vetorIdaVolta(_MGGPPAuxEdges,sourceEdge)&&!aresta_no_vetorIdaVolta(visitedEdges,sourceEdge)){
                cutEdges.push_back(sourceEdge);
          //      cout<<"adicionado edge:"<<sourceEdge->_source_id<<"-"<<sourceEdge->_target_id<<endl;
            }
            sourceEdge=sourceEdge->_next_edge;
        } 
          Node* target = search_for_node(edge->_target_id);
         Edge* targetEdge = target->_first_edge;
        while(targetEdge!=nullptr){
            if(!aresta_no_vetorIdaVolta(cutEdges,targetEdge)&&!aresta_no_vetorIdaVolta(_MGGPPAuxEdges,targetEdge)&&!aresta_no_vetorIdaVolta(visitedEdges,targetEdge)){
                cutEdges.push_back(targetEdge);
            //    cout<<"adicionado edge:"<<targetEdge->_source_id<<"-"<<targetEdge->_target_id<<endl;

            }
            targetEdge=targetEdge->_next_edge;
        
        // if(!aresta_no_vetorIdaVolta(coveredEdges,edge)){
        //     cutEdges.push_back(edge);
        // }
    }
       
    }

    return cutEdges;
}

// pagina 3 do paper
vector<Edge*> Graph::AlgoritmoGuloso(){
        auto start = std::chrono::system_clock::now();

    vector<MGGPPAux> x;
    vector<Edge*> arestasDescobertas;
    vector<Node*> vertices_descobertos;
    vector<Edge*> arestas = order_edges_non_decreasing_gap();
    int total_gap = 0;
    int p = this->numeroSubGrafos;
    Edge* escolhida;
    int contador=0;
    cout<<"fez config inicial"<<endl;
   
     MGGPPAux* m=  new MGGPPAux();
            x.push_back(*m);
    while(x.size() <=p){ // padrão do algoritmo
        for(int i = 0; i < arestas.size(); i++){ // extract_first(E\X)
            if(!aresta_no_vetorIdaVolta(arestasDescobertas, arestas[i])){
                escolhida = arestas[i];
                cout<<"escolhida"<<escolhida->_source_id<<"- "<<escolhida->_target_id<<endl;
                auto iterador = arestas.at(i);
                arestas.erase(remove(arestas.begin(), arestas.end(), arestas[i]), arestas.end()); // remove a aresta que a gente pegou
                 break;
            }
        }
        if(!arestas_adj(arestasDescobertas, escolhida)){ // se a escolhida nao é adjacente a ngm que ta em X
            Node* source = search_for_node(escolhida->_source_id);
                vertices_descobertos.push_back(source);          
            Node* target = search_for_node(escolhida->_target_id);
                vertices_descobertos.push_back(target);         
            MGGPPAux* m=  new MGGPPAux();
            x.push_back(*m);                      
            x[contador].edges.push_back(escolhida);
            size_t currentMaxValue = max(source->_weight,target->_weight);
            x[contador].maxvalue = currentMaxValue;
            size_t currentMinValue = min(source->_weight,target->_weight);
            x[contador].minvalue = currentMinValue;
            x[contador].gap= x[contador].maxvalue- x[contador].minvalue;
            contador++;
            arestasDescobertas.push_back(escolhida);
            total_gap += determinar_gap_aresta(escolhida);
        }
    }
    x.pop_back();
    // for(int i=0;i<vertices_descobertos.size();i++){
    //     cout<<"vertice descoberto:"<<vertices_descobertos[i]->_id<<endl;
    // }
    while (vertices_descobertos.size() < this->_number_of_nodes){
      //  cout<<"Processando vertice numero:"<<vertices_descobertos.size()<<endl;
          Edge* melhor;
    int posMelhor=0;
    int custoAdicionalMelhor=infinito;
   // cout<<"verticesdescobertos"<<vertices_descobertos.size()<<"numeroDeNos"<<this->_number_of_nodes;
int tipo=0;
int tipoMelhor=0;
            for(int i=0;i<x.size();i++){
                vector<Edge*> ArestasCortadas = getCutEdges(x[i].edges, arestasDescobertas);
        for ( Edge* e : ArestasCortadas) {
            int custoAdicionalCorrente=infinito;
           auto source_node = search_for_node(e->_source_id);
auto target_node = search_for_node(e->_target_id);
size_t custoMaximoCorrente = max(source_node->_weight, target_node->_weight);
size_t custoMinimoCorrente = min(source_node->_weight, target_node->_weight);

          //  cout<<"aresta"<<e->_source_id<<"-"<<e->_target_id<<"custo Adicional mellhor"<<custoAdicionalMelhor<<endl;
          //custoAdicionalCorrente = max(custoMaximoCorrente,x[i].maxvalue)-min(custoMinimoCorrente,x[i].minvalue);
             if(x[i].maxvalue>=custoMaximoCorrente&&x[i].minvalue<=custoMinimoCorrente){
                tipo=1;
                custoAdicionalCorrente=x[i].maxvalue-x[i].minvalue;//fazer um modo de quebrar aqui pois ja é a melhor opção possível
             }else if(x[i].maxvalue<=custoMaximoCorrente&&x[i].minvalue>=custoMinimoCorrente){
                custoAdicionalCorrente =  custoMaximoCorrente-custoMinimoCorrente;
            tipo=2;
             }
             else if(x[i].maxvalue<=custoMaximoCorrente&&x[i].minvalue<=custoMinimoCorrente){
                tipo=3;
                custoAdicionalCorrente = custoMaximoCorrente-x[i].minvalue;
             }else if(x[i].maxvalue>=custoMaximoCorrente&&x[i].minvalue>=custoMinimoCorrente){
                tipo=4;
            custoAdicionalCorrente = x[i].maxvalue-custoMinimoCorrente;
             }else{
                cout<<"caiu no else";
             }
            //    cout<<"max value:"<<x[i].maxvalue<<"min value"<<x[i].minvalue;
            //    cout<<"custoMaximoCorrente"<<custoMaximoCorrente<<"custoMinimoCorrente"<<custoMinimoCorrente;
            //    cout<<"custoAdicionalMelhor"<<custoAdicionalMelhor<<"custoAdicionalCorrente";
            //     cout<<"teste"<<"custo maximo corrente:"<<custoMaximoCorrente<<"-"<<custoMinimoCorrente;
            //    cout<<"custo Adicional Corrente:"<<custoAdicionalCorrente<<"custo adicional melhor"<<custoAdicionalMelhor;
            //  
         //cout<<"custo adicional:"<<custoAdicionalCorrente<<endl;
            //cout<<"tamanho arestas"<<ArestasCortadas.size()<<"tamanho arestas conhecidas"<<arestasDescobertas.size();
             if(custoAdicionalMelhor>custoAdicionalCorrente){
                custoAdicionalMelhor=custoAdicionalCorrente;
                posMelhor =i;
                melhor =e;
                tipoMelhor=tipo;
                if(tipo==1){break;}
                 //   cout<<"custo minimo"<<custoMaximoCorrente-custoMinimoCorrente<<"custo maximo:"<<custoMaximoCorrente<<"custo minimo:"<<custoMinimoCorrente;
                
              //  cout<<"melhor definido"<<melhor->_source_id<<"-"<<melhor->_target_id<<"custo"<<custoAdicionalMelhor<<"para o subespaco:"<<1<<endl;
             }
            }
         // cout<<"e:"<<e->_source_id<<"-"<<e->_target_id;
        }

       // cout<<"melhor final:"<<melhor->_source_id<<"-"<<melhor->_target_id<<"subespaco:"<<posMelhor<<"Custo adicional:"<<custoAdicionalMelhor<<"tipo"<<tipoMelhor<<endl;
           Node* source = search_for_node(melhor->_source_id);
        Node* target = search_for_node(melhor->_target_id);
         if(!node_no_vetor(vertices_descobertos, target) ||!node_no_vetor(vertices_descobertos, source)){
        size_t custoMaximoMelhor = max(source->_weight, target->_weight);
            size_t custoMinimoMelhor = min(source->_weight, target->_weight);
           // cout<<"Peso 1:"<<source->_weight<<"Peso 2:"<<target->_weight<<"minimo"<<min(source->_weight, target->_weight)<<"Valor atual no min:"<<x[posMelhor].minvalue<<endl;
            x[posMelhor].edges.push_back(melhor);
            x[posMelhor].maxvalue  = max(custoMaximoMelhor,x[posMelhor].maxvalue);
             x[posMelhor].minvalue  = min(custoMinimoMelhor,x[posMelhor].minvalue);
             x[posMelhor].gap = x[posMelhor].maxvalue- x[posMelhor].minvalue;
        }              
            arestasDescobertas.push_back(melhor);
                if (!node_no_vetor(vertices_descobertos, source))
                {
            vertices_descobertos.push_back(source);
                }    
               if (!node_no_vetor(vertices_descobertos, target))
               {
            vertices_descobertos.push_back(target);

                }
        }
            auto end = std::chrono::system_clock::now();
              std::chrono::duration<double> elapsed_seconds = end-start;
        int gapFinal =0;
        for (size_t i = 0; i < x.size(); i++)
        {
            gapFinal+=x[i].gap;
            cout<<"gap"<<x[i].gap<<endl;
      }
        
    
    // for(int i=0;i<x.size();i++){
    //     cout<<"x:"<<abs(search_for_node(x[i]->_source_id)->_weight-search_for_node(x[i]->_target_id)->_weight)<<endl;
    // }
    // for(int i=0;i<vertices_descobertos.size();i++){
    //     //cout<<"vertice descoberto:"<<vertices_descobertos[i]->_id<<endl;

    // }
    vector<size_t> nodesJaImpressos;
    string color[5] = {"blue","green","yellow","magenta","red"};
    for(int i =0;i<x.size();i++){
        // cout<<"subespaco:"<<i<<endl;
        // cout<<"valor máximo:"<<x[i].maxvalue<<endl;
        // cout<<"valor mínimo"<<x[i].minvalue<<endl;
        // cout<<"gap:"<<x[i].gap<<endl;
        for(int j=0;j<x[i].edges.size();j++){
            if(!ta_no_vetor(nodesJaImpressos,x[i].edges[j]->_target_id)){

            
            //   cout<<"<node id=\""<<x[i].edges[j]->_target_id<<"\">"<<endl;
            //  cout<<"<data key=\"d0\">"<<color[i]<<"</data>"<<endl;
            //  cout<<"    </node>"<<endl;
            //  nodesJaImpressos.push_back(x[i].edges[j]->_target_id);
            //  }if(!ta_no_vetor(nodesJaImpressos,x[i].edges[j]->_source_id)){

            //   cout<<"<node id=\""<<x[i].edges[j]->_source_id<<"\">"<<endl;
            //  cout<<"<data key=\"d0\">"<<color[i]<<"</data>"<<endl;
            //  cout<<"    </node>"<<endl;
            //               nodesJaImpressos.push_back(x[i].edges[j]->_source_id);

             }
        //   cout<<" contem o edge"<<x[i].edges[j]->_source_id<<"-"<<x[i].edges[j]->_target_id<<endl;
        cout<<"<edge source=\""<<x[i].edges[j]->_source_id<<"\""<<" "<<"target=\""<<x[i].edges[j]->_target_id<<"\"/>"<<endl;

        }
    }
    cout<<"Gap final encontrado:"<<gapFinal<<endl;
    cout<<x.size();
    cout<<endl<< "elapsed time: " << elapsed_seconds.count() << "s";

    return x[0].edges;
}

    

size_t gap(size_t weightOne,size_t weightTwo){
    if(weightOne>=weightTwo){
        return weightOne-weightTwo;
    }
    return weightTwo-weightOne;

}
bool Graph::isAdjacent(size_t first_id,size_t second_id){
    Node* firstNode = search_for_node(first_id);
    Edge* edgeTraversal = firstNode->_first_edge;
    while(edgeTraversal!=nullptr){
        if(edgeTraversal->_target_id==second_id){
            return true;
        }
        edgeTraversal=edgeTraversal->_next_edge;
    }
    return false;
}
bool hasUncoveredVertices(){
    return true;
}
Graph::Graph()
{
}

Graph::~Graph()
{
    Node* no = this->_first;
    Node* nextNode = nullptr;
    while (no != nullptr) {
        Edge* aresta = no->_first_edge;
        Edge* nextEdge = nullptr;
        int i = 0;
        while (aresta != nullptr) {
            nextEdge = aresta->_next_edge;
            delete aresta;
            aresta = nextEdge;
            this->_number_of_edges--;
            i++;
        }
        nextNode = no->_next_node;
        delete no;
        no = nextNode;
        this->_number_of_nodes--;
    }

    // cout << "Edges: " << this->_number_of_edges << endl;
    // if (this->_number_of_edges > 0) {
    //     cout << "ta sobrando";
    // }
    // cout << "\nNodes: " << this->_number_of_nodes << endl;
}

void Graph::remove_node(size_t node_position)
{
    Node* nodeToRemove = search_for_node(node_position);
    if (nodeToRemove == nullptr) {
        cout << "Nó não encontrado no grafo!" << endl;
        return;
    }

    // Remover todas as arestas conectadas ao nó
    while (nodeToRemove->_first_edge != nullptr) {
        remove_edge(node_position, nodeToRemove->_first_edge->_target_id);
    }

    // Remover o nó da lista de nós
    if (nodeToRemove == this->_first) {
        this->_first = nodeToRemove->_next_node;
        if (this->_first != nullptr) {
            this->_first->_previous_node = nullptr;
        }
    } else if (nodeToRemove == this->_last) {
        this->_last = nodeToRemove->_previous_node;
        if (this->_last != nullptr) {
            this->_last->_next_node = nullptr;
        }
    } else {
        nodeToRemove->_previous_node->_next_node = nodeToRemove->_next_node;
        if (nodeToRemove->_next_node != nullptr) {
            nodeToRemove->_next_node->_previous_node = nodeToRemove->_previous_node;
        }
    }

    delete nodeToRemove;
    this->_number_of_nodes--;
    cout << "Nó removido com sucesso!" << endl;
}
void Graph::remove_edge(size_t node_position_1, size_t node_position_2)
{
    Node* No1 = search_for_node(node_position_1);
    if (No1 == nullptr) {
        cout << "Nó de origem não encontrado no grafo!" << endl;
        return;
    }

    Node* No2 = search_for_node(node_position_2);
    if (No2 == nullptr) {
        cout << "Nó de destino não encontrado no grafo!" << endl;
        return;
    }

    // Remover a aresta do nó de origem
    Edge* prevEdge = nullptr;
    Edge* edgeTraversal = No1->_first_edge;

    while (edgeTraversal != nullptr) {
        if (edgeTraversal->_target_id == node_position_2) {
            if (prevEdge == nullptr) {
                No1->_first_edge = edgeTraversal->_next_edge;
            } else {
                prevEdge->_next_edge = edgeTraversal->_next_edge;
            }
            delete edgeTraversal;
            No1->_number_of_edges--;
            this->_number_of_edges--;
            cout << "Aresta removida do nó de origem com sucesso!" << endl;
            break;
        }
        prevEdge = edgeTraversal;
        edgeTraversal = edgeTraversal->_next_edge;
    }

    // Remover a aresta do nó de destino (apenas para grafos não direcionados)
    if (!this->_directed) {
        prevEdge = nullptr;
        edgeTraversal = No2->_first_edge;

        while (edgeTraversal != nullptr) {
            if (edgeTraversal->_target_id == node_position_1) {
                if (prevEdge == nullptr) {
                    No2->_first_edge = edgeTraversal->_next_edge;
                } else {
                    prevEdge->_next_edge = edgeTraversal->_next_edge;
                }
                delete edgeTraversal;
                No2->_number_of_edges--;
                cout << "Aresta removida do nó de destino com sucesso!" << endl;
                break;
            }
            prevEdge = edgeTraversal;
            edgeTraversal = edgeTraversal->_next_edge;
        }
    }
}


void Graph::add_node(size_t node_id, float weight)
{
    if(this->_first==nullptr){
        Node* firstNode = new Node();
        _first = firstNode;
        _first->_id = node_id;
        firstNode->_visitado = false;
        firstNode->_weight=weight;
        this->_number_of_nodes++;
    }else{
        Node* aux=_first;
        bool jaExiste=false;
        if(aux->_id==node_id){
            jaExiste=true;
        }
        while(aux->_next_node!=nullptr){     
            aux = aux->_next_node;
            if(aux->_id==node_id){
                jaExiste=true;
                break;
            }
        }
        if(jaExiste){
            return;
        }
        aux->_next_node = new Node();
        aux->_next_node->_id = node_id;
        this->_last = aux->_next_node;
        aux->_next_node->_visitado = false;
        aux->_next_node->_weight=weight;
        this->_number_of_nodes++;
    }
}

void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight, bool _gemea){  // default weight = 1, _gemea = false
    Node* No1;
    Node* traversal=this->_first;
    //adj[node_id_1].push_back(make_pair(node_id_2, weight));
    while(traversal!=nullptr){
        if(node_id_1==traversal->_id){
            No1 = traversal;
            break;
        }else{
            traversal=traversal->_next_node;
        }
    }
    if(No1->_first_edge==nullptr){
        No1->_first_edge = new Edge();

        No1->_first_edge->_source_id = No1->_id; // check me pablo!
        No1->_first_edge->_weight = weight;
        No1->_first_edge->_target_id = node_id_2;
        No1->_first_edge->_gemea = _gemea;
        No1->_number_of_edges++; // check me
        this->_number_of_edges++;
        //    cout<<"Criando primeira aresta do no"<<No1->_id<<endl;
    }else{
        Edge* edgeTraversal=traversal->_first_edge;
        while(edgeTraversal->_next_edge!=nullptr){
            //  cout<<"Contem aresta:"<<edgeTraversal->_target_id<<endl;
            edgeTraversal=edgeTraversal->_next_edge;
        }
        edgeTraversal->_next_edge = new Edge();

        edgeTraversal->_next_edge->_source_id = node_id_1;
        edgeTraversal->_next_edge->_weight = weight;
        edgeTraversal->_next_edge->_target_id = node_id_2;
        edgeTraversal->_next_edge->_gemea = _gemea;
        this->_number_of_edges++;
        No1->_number_of_edges++; // check me

        //   cout<<"Criando aresta do no"<<No1->_id<<endl;
    }
  //  cout<<"aresta Criado"<<endl;
}

void Graph::print_graph()
{
    cout<<"Imprimindo...";
    cout<<"Primeiro no:"<<this->_first->_id<<endl;
    Node* traversal=this->_first;
    Edge* edgeTraversal = traversal->_first_edge;
    while(traversal!=nullptr){
        cout<<"Contem no:"<<traversal->_id << "\t Peso = " << traversal->_weight<<endl;
        edgeTraversal = traversal->_first_edge;
        while(edgeTraversal!=nullptr){
            cout<<"     Contem aresta:"<<traversal->_id<<"-"<<edgeTraversal->_target_id << " (" << edgeTraversal->_weight<<")"<<endl;
           edgeTraversal=edgeTraversal->_next_edge;
        }
        traversal=traversal->_next_node;
    }
    cout<<"Ultimo no:" << this->_last->_id<<endl;
    cout<<"Numero de Vertices:" << this->_number_of_nodes << endl;
    cout<<"Numero de Arestas:" << this->_number_of_edges << endl;
}

pair<size_t,string> Graph::dijkstra(size_t origem, size_t destino){
    if(search_for_node(origem)==nullptr||search_for_node(destino)==nullptr)
    {
        return {0,""};
    }  
    size_t distancias[this->_number_of_nodes];
    size_t visitados[this->_number_of_nodes];
    int predecessor[this->_number_of_nodes];
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fila;
    
    for(size_t i = 0; i <= this->_number_of_nodes; i++)
		{
			distancias[i] = infinito;
			visitados[i] = false; //mudar para 0 ou 1
            predecessor[i] = -1;  // -1 indica que o nó não tem predecessor
		}
    distancias[origem] = 0;
    fila.push(make_pair(distancias[origem], origem));
    while(!fila.empty())
	{
		pair<int, int> p = fila.top(); 
		int vertice = p.second;
		fila.pop(); 

		if(visitados[vertice] == false){
		    visitados[vertice] = true;
	        for (size_t i = 0; i < adj[vertice].size(); i++) {
                size_t v = adj[vertice][i].first;
                size_t custo_da_aresta = adj[vertice][i].second;
                if (distancias[v] > (distancias[vertice] + custo_da_aresta)) {
                    distancias[v] = distancias[vertice] + custo_da_aresta;
                    predecessor[v] = vertice;  // Atualiza o predecessor
                    fila.push(make_pair(distancias[v], v));
                }
            }
		}
	}
    string caminho;
    for (int at = destino; at != -1; at = predecessor[at]) {
        caminho=to_string(at)+ (caminho.empty() ? "" : " -> ")+caminho;
    }
    return {distancias[destino],caminho};
	
}
void Graph::print_graph(std::ofstream& output_file)
{
}

int Graph::conected(size_t node_id_1, size_t node_id_2)
{
    // passar de nó em nó, conferir se tem mais de 1 aresta e passar de aresta em aresta ate achar o alvo
    // retorna 1 pra sim e 0 pra não. apenas pra nao trocar o retorno da função
    // NOTA: **Aparentemente** funciona pra grafos direcionados também
    if (this->_first == nullptr){ // aqui nao tem nem o primeiro nó, então ja para
        return 0;
    }
    Node *no;
    Edge *aresta;
    Node *traversal = this->_first;
    while (traversal != nullptr){
        if (traversal->_id == node_id_1){
            no = traversal;
            aresta = no->_first_edge;
            for (size_t i = 0; i < no->_number_of_edges; i++){
                if (aresta->_target_id == node_id_2){
                    return 1;
                }
                aresta = aresta->_next_edge;
            }
        }
        traversal = traversal->_next_node;
    }
    return 0;
}


vector<size_t> Graph::fecho_tran_direto(size_t node_id, vector<Edge*>& retArestas){
    vector<size_t> final = this->arvore_caminho_profundidade(node_id, retArestas);
    final.erase(final.begin());
    if(final.size() > 0){
        cout<<"Fecho transitivo direto do vértices "<<node_id<<" constituído pelos vértices: "; 
        for (auto i : final){
            cout << i << " ";
        }
    } else {
        cout << "O fecho transitivo direto do nó " << node_id << " é vazio. \n";
    }
    return final;
}

vector<size_t> Graph::fecho_tran_indireto(size_t node_id, vector<Edge*>& retArestas){
    vector<size_t> retorno;
    vector<size_t> caminhoDoNo;
    bool adicionar;
    for(Node* no = this->_first; no != nullptr; no = no->_next_node){
        adicionar = false;
        if(no->_id == node_id){ // ignora o próprio nó
            continue;
        }
        
        caminhoDoNo = arvore_caminho_profundidade(no->_id, retArestas); // faz a busca em profundidade de no->id
        
        for(size_t elemento : caminhoDoNo){
            if(elemento == node_id){
                adicionar = true; // se achar um igual ao alvo, quer dizer que o no enxerga o ele, entao sinaliza
            }
        }
        if(adicionar){
            retorno.push_back(no->_id);
        }
    }
    if(retorno.size() > 0){
        cout << "O fecho transitivo indireto do vértice " << node_id << " é constituido pelos vértices: ";
        for(auto i : retorno){
            cout << i << " ";
        }
    } else {
        cout << "O fecho transitivido indireto do vértice " << node_id << " é vazio.\n";
    }
    return retorno;
}

// Sempre que usar isso, confira se o retorno foi nullptr! Se nao for e voce tentar usar algum atributo,
// vai dar core dumped
// ex:
// Node* busca = grafo->search_for_node(6);
// if(busca != nullptr){
//     cout << busca->_id << endl;
// }
Node* Graph::search_for_node(size_t node_id){ //busca um nó no grafo, se achar retorna o endereço dele
    Node* no = this->_first;

    while(no!=nullptr){     
        if (no->_id == node_id){
            return no;
        }
        no = no->_next_node;   
    }
    return nullptr;
}

bool Graph::ta_no_vetor(vector<size_t>& vetor, size_t node_id){
    for (size_t i = 0; i < vetor.size(); i++){   
        if(vetor[i]==node_id){
            return true;
        }
    }
    return false;
}

bool Graph::node_no_vetor(vector<Node*>& vetor, Node* node){
    for (size_t i = 0; i < vetor.size(); i++){
        if(vetor[i]==node){
            return true;
        }
    }
    return false;
}
bool Graph::aresta_no_vetor(vector<Edge*>& vetor, Edge* aresta){
       return find(vetor.begin(), vetor.end(), aresta) != vetor.end();

}
bool areEqualEdges(Edge* e1, Edge* e2) {
    return (e1->_source_id == e2->_source_id && e1->_target_id == e2->_target_id) ||
           (e1->_source_id == e2->_target_id && e1->_target_id == e2->_source_id);
}
bool Graph::aresta_no_vetorIdaVolta(vector<Edge*>& vetor, Edge* aresta){   
         bool encontrada = false;
    for (Edge* e : vetor) {
        if (areEqualEdges(e, aresta)) {
            encontrada = true;
            break;
        }
    }

    // Debug para verificar o resultado
  //  cout << "Aresta encontrada: " << encontrada << endl;

    return encontrada;
}
bool Graph::getDirected(){
    return this->_directed;
}
bool Graph::getWeighted_edges(){
    return this->_weighted_edges;
}
bool Graph::getWeighted_nodes(){
    return this->_weighted_nodes;
}

size_t Graph::getNumberOfNodes(){
    return this->_number_of_nodes;
}

bool Graph::taNoGrafo(size_t no){
    if (this->search_for_node(no) == nullptr){
        return false;
    }
    return true;
}

vector<Edge*> Graph::gerarVerticeInduzido(vector<size_t> vertices){
    // faz um fuzuê entre arrays e retorna as edges que geram um sub Vert induzido
    // com os IDs fornecidos.
    // supoe-se que ja foi feita a validação quanto a repetição dos vertices e quanto a existencia deles no grafo
    vector<Edge*> retorno;
    vector<Node*> nos;
    for (size_t i : vertices){ // gera um vetor de nos
        Node* no = this->search_for_node(i);
        nos.push_back(no);
    }
    for(Node* i : nos){
        Edge* aresta = i->_first_edge;
        while(aresta!=nullptr){
            if (this->ta_no_vetor(vertices, aresta->_target_id)){
                retorno.push_back(aresta);
            }
            aresta = aresta->_next_edge;
        }
    }
    // cout << retorno.size() << endl;
    return retorno;
}

vector<Edge*> Graph::agmPrim(vector<Edge*> arestas, size_t nNos){
// LEMBRETE: a intenção era fazer um array com as arestas disponíveis (arestas disponíveis = arestas ligadas aos vertices
// que ja foram inseridos) e iterar sobre ela pra achar a proxima a ser adicionada
      vector<Edge*> arvoreGeradoraMinima;
    vector<int> visitados;
    vector<Edge*> FilaArestas;
    size_t peso_total = 0;

    // Inicialização: escolha um nó inicial
    Edge* inicial = arestas[0];
    visitados.push_back(inicial->_source_id);
    
    // Adiciona todas as arestas conectadas ao nó inicial
    for (Edge* e = inicial; e != nullptr; e = e->_next_edge) {
        FilaArestas.push_back(e);
    }
    
    while (visitados.size() < nNos) {
        // Ordena as arestas por peso
        sort(FilaArestas.begin(), FilaArestas.end(), [](Edge* e1, Edge* e2) {
            return e1->_weight < e2->_weight;
        });

        // Remove a aresta de menor peso
        Edge* minEdge = FilaArestas.front();
        FilaArestas.erase(FilaArestas.begin());

        if (NoNoVetor(visitados, minEdge->_source_id) && NoNoVetor(visitados, minEdge->_target_id)) {
            continue; // Ambos os nós já foram visitados
        }

        // Adiciona a aresta à árvore geradora mínima
        arvoreGeradoraMinima.push_back(minEdge);
        peso_total += minEdge->_weight;

        // Adiciona o nó não visitado
        int novoNo = NoNoVetor(visitados, minEdge->_source_id) ? minEdge->_target_id : minEdge->_source_id;
        visitados.push_back(novoNo);

        // Adiciona todas as arestas conectadas ao novo nó à fila de arestas
        for (Edge* e = search_for_node(novoNo)->_first_edge; e != nullptr; e = e->_next_edge) {
            if (!NoNoVetor(visitados, e->_source_id) || !NoNoVetor(visitados, e->_target_id)) {
                FilaArestas.push_back(e);
            }
        }
    }

    // Exibe a árvore geradora mínima
    cout << "Árvore Geradora Mínima:" << endl;
    for ( Edge* e : arvoreGeradoraMinima) {
        cout << "(" << e->_source_id << " - " << e->_target_id << ") Peso: " << e->_weight << endl;
    }
    cout << "Peso Total: " << peso_total << endl;

    return arvoreGeradoraMinima;

    
}
bool Graph::NoNoVetor(vector<int> nos, int id){
    for(int i =0;i<nos.size();i++){
        if(nos[i]==id){
            return true;
        }
    }return false;
}


vector<Edge*> Graph::agmKruskal(vector<Edge*> arestas, size_t n){
    // bota em ordem por peso
     // Ordena as arestas por peso
    sort(arestas.begin(), arestas.end(), [](Edge* a1, Edge* a2) {
        return a1->_weight < a2->_weight;
    });

    vector<Edge*> retorno; // Arestas que serão retornadas
    size_t peso_total = 0;
    Conjunto conj(n);

    for (Edge* aresta : arestas) {
        if (aresta->_gemea) {
            // Pular arestas gêmeas
            continue;
        }
        // Verifica se a aresta cria um ciclo
        if (conj.find(aresta->_source_id) != conj.find(aresta->_target_id)) {
            conj.unite(aresta->_source_id, aresta->_target_id);
            retorno.push_back(aresta);
            peso_total += aresta->_weight;
        }
    }

    // Ordena as arestas no retorno por ID da fonte
    sort(retorno.begin(), retorno.end(), [](Edge* a1, Edge* a2) {
        return a1->_source_id < a2->_source_id;
    });

    // Exibe o resultado
    cout << "Árvore Geradora Mínima composta pelas arestas\n";
    for (Edge* i : retorno) {
        cout << "(" << i->_source_id << ", " << i->_target_id << ") ";
    }
    cout << "\nCusto total: " << peso_total << endl;

    return retorno;
}

vector<size_t> Graph::arvore_caminho_profundidade(size_t noInicial, vector<Edge*>& retArestas){
    // inicializa tudo em falso por garantia --> se so o construtor fizer isso, a segunda vez que for usar o [7] vai dar errado
    vector<size_t> retorno;
    this->desvisitar_todos();
    this->caminho_profundidade(retorno, noInicial, retArestas);
    return retorno;
}

void Graph::caminho_profundidade(vector<size_t> &retorno, size_t noInicial, vector<Edge*>& retArestas){
    Node* no = search_for_node(noInicial);
    no->_visitado = true;
    retorno.push_back(no->_id);
    for(Edge* aresta = no->_first_edge; aresta != nullptr; aresta=aresta->_next_edge){
        Node* aux = search_for_node(aresta->_target_id);
        if (!aux->_visitado){
            if(!aresta_no_vetor(retArestas, aresta))
                retArestas.push_back(aresta);
            caminho_profundidade(retorno, aux->_id, retArestas);
        }
    }
}

void Graph::determinar_excentricidades(){
    
    for(size_t i = 1; i < this->_number_of_nodes; i++){
        size_t exc = 0;
        for(size_t j = 1; j < this->_number_of_nodes; j++){
            if(i != j){ // nao analisa iguais
                pair<size_t, string> dijkstra = this->dijkstra(i,j); // determina menor caminho
                
                if(dijkstra.first != infinito){
                    if (dijkstra.first > exc){
                        exc = dijkstra.first;
                    }
                }
            }
        }
        this->excentricidades[i] = exc;
    }
}
unordered_map<size_t, size_t> Graph::getExcentricidades(){
    return this->excentricidades;
}

void Graph::determinar_raio(){ 
    for(auto& par : this->excentricidades){
        if(par.second != 0 && par.second < this->raio){
            this->raio = par.second;
        }
    }
}
size_t Graph::get_raio(){
    return this->raio;
}

void Graph::determinar_diametro(){ 
    for(auto& par : this->excentricidades){
        if(par.second != infinito){
            if(par.second > this->diametro){
                this->diametro = par.second;
            }
        }
    }
}
size_t Graph::get_diametro(){
    return this->diametro;
}

void Graph::determinar_centro(){
    // == raio
    for (size_t i = 1; i <= this->_number_of_nodes; i++){
        if(this->excentricidades[i] == this->raio){
            this->centro.push_back(i);
        }
    }
}
vector<size_t> Graph::getCentro(){
    return this->centro;
}

void Graph::determinar_periferia(){
    // == diametro
    for (size_t i = 1; i <= this->_number_of_nodes; i++){
        if(this->excentricidades[i] == this->diametro){
            this->periferia.push_back(i);
        }
    }
}
vector<size_t> Graph::getPeriferia(){
    return this->periferia;
}

Node* Graph::getFirst(){
    return this->_first;
}

void Graph::lista_adjacencia(ofstream& arquivo_saida){ // printa a lista de adj do grafo e salva ele no arquivo de saida (txt) fornecido
    arquivo_saida << "\n====== Lista de Adjacência ======\n";
    cout << endl;
    for(Node* no = this->_first; no != nullptr; no = no->_next_node){
        cout << no->_id << endl;
        if(no->_next_node != nullptr){
            cout << no->_id << " -> ";
            arquivo_saida << no->_id << " -> ";
        } else {
            cout << no->_id << " -> X\n";
            arquivo_saida << no->_id << " -> X\n";
        }
        for(Edge* aresta = no->_first_edge; aresta != nullptr; aresta = aresta->_next_edge){
            if(aresta->_next_edge == nullptr){
                cout << aresta->_target_id << " -> X ";
                arquivo_saida << aresta->_target_id << " -> X \n";
            } else {
                cout << aresta->_target_id << " -> ";
                arquivo_saida << aresta->_target_id << " -> ";
            }
        }
        cout << endl;
    }
}

void Graph::desvisitar_todos(){
    for(Node* no = this->_first; no!=nullptr; no = no->_next_node){
        no->_visitado = false;
    }
}

pair<size_t,string> Graph::floyd(size_t inicio, size_t destino){//size_t inicial, size_t destino){
    
    size_t n = this->_number_of_nodes;
    // vector<Edge*> passou_por_onde;
    vector<vector<float>> matrizFloyd(n+1, vector<float>(n+1, infinito)); // cria matriz de tamanho n+1 e infinito em todo mundo
    vector<vector<int>> next(n+1, vector<int>(n+1, -1)); // Matriz para reconstrução do caminho
    for (size_t i = 1; i <= n; i++) {
        matrizFloyd[i][i] = 0;
    }
    // reiniciar distancias
    for (Node* node = this->_first; node != nullptr; node = node->_next_node) {
        size_t i = node->_id;
        Edge* edge = node->_first_edge;
        while (edge) {
            size_t j = edge->_target_id;
            matrizFloyd[i][j] = edge->_weight;
            next[i][j] = j;

            if (edge->_gemea) {
                // nao direcionado
                matrizFloyd[j][i] = edge->_weight;
                next[j][i] = i;

            }
            edge = edge->_next_edge;
        }
    }
    // vector<Edge*> allArestas = this->allEdges();
    
       for (size_t k = 1; k <= n; k++) {
            for (size_t i = 1; i <= n; i++) {
                for (size_t j = 1; j <= n; j++) {
                    if (matrizFloyd[i][k] < infinito && matrizFloyd[k][j] < infinito) {
                        float newDist = matrizFloyd[i][k] + matrizFloyd[k][j];
                        if (newDist < matrizFloyd[i][j]) {
                            matrizFloyd[i][j] = newDist;
                            next[i][j] = next[i][k];
                        }
                    }
                }
            }
        }auto getPath = [&](int i, int j) {
       vector<int> path;
            if (next[i][j] == -1) return path; // Se não há caminho
            path.push_back(i);
            while (i != j) {
                i = next[i][j];
                path.push_back(i);
            } return path;
        };  
        string caminhoMenorCusto;
                if (inicio != destino) {
                    vector<int> path = getPath(inicio, destino);
                    if (!path.empty()) {
                        for (int node : path) {
                 caminhoMenorCusto=caminhoMenorCusto+ (caminhoMenorCusto.empty() ? "" : " -> ")+to_string(node);
                        }
                    }
              
        }
            return {matrizFloyd[inicio][destino],caminhoMenorCusto};
    
}


vector<Edge*> Graph::allEdges(){
    vector<Edge*> retorno;
    for(Node* no = this->_first; no!= nullptr; no = no->_next_node){
        for(Edge* edge = no->_first_edge; edge!=nullptr; edge=edge->_next_edge){
            retorno.push_back(edge);
        }
    }
    return retorno;
}

Edge* Graph::getAresta(size_t no1, size_t no2){
    Edge* aresta = this->_first->_first_edge;
    while(aresta != nullptr){
        if(aresta->_source_id == no1 && aresta->_target_id == no2){
            return aresta;
        }
        aresta = aresta->_next_edge;
    }
    return nullptr;
}

// feita pra exportar no formato do graphviz
// void Graph::exportar(vector<Edge*> arestas, ofstream& arquivo_saida){
//     // fazer uma lista de adjacência com as arestas

//     vector<size_t> idNos;
//     // pega os ids envolvidos nas arestas
//     for(Edge* aresta : arestas){
//         if(!ta_no_vetor(idNos, aresta->_source_id)){
//             idNos.push_back(aresta->_source_id);
//         }
//         if(!ta_no_vetor(idNos, aresta->_target_id)){
//             idNos.push_back(aresta->_target_id);
//         }
//     }
//     // organiza os ids em ordem crescente pra ficar bonito fofo e organized
//     sort(idNos.begin(), idNos.end(), [](size_t id1, size_t id2){return id1 < id2;});
//     // aqui começa as verificações pra ver como vai fazer o .dot
//     if(this->_directed){
//         arquivo_saida << "digraph G {\n";
//     } else {
//         arquivo_saida << "graph G {\n";
//     }
//     for(size_t id : idNos){

//         for(Edge* aresta : arestas){

//             if(aresta->_source_id == id){
//                 arquivo_saida << "\t" << id; // linha do nó do momento
//                 if(this->_directed){
//                     arquivo_saida << " -> " << aresta->_target_id << "[label=\"" << aresta->_weight << "\"];\n";
//                     cout << " -> " << aresta->_target_id;
//                 } else {
//                     arquivo_saida << " -- " << aresta->_target_id << "[label=\"" << aresta->_weight << "\"];\n";
//                     cout << " -- " << aresta->_target_id;
//                 }

//             }
//             // if(aresta->_target_id == id){
//             //     if(!this->_directed){
//             //         arquivo_saida << " -- " << aresta->_source_id;
//             //         cout << " -- " << aresta->_target_id;
//             //     }
//             // }

//         }
//     }
//     arquivo_saida << "}";
// }

void Graph::exportar(vector<Edge*> arestas, ofstream& arquivo_saida){
    // fazer uma lista de adjacência com as arestas

    vector<size_t> idNos;
    // pega os ids envolvidos nas arestas
    for(Edge* aresta : arestas){
        if(!ta_no_vetor(idNos, aresta->_source_id)){
            idNos.push_back(aresta->_source_id);
        }
        if(!ta_no_vetor(idNos, aresta->_target_id)){
            idNos.push_back(aresta->_target_id);
        }
    }
    // organiza os ids em ordem crescente pra ficar bonito fofo e organized
    sort(idNos.begin(), idNos.end(), [](size_t id1, size_t id2){return id1 < id2;});
    // aqui começa as verificações pra ver como vai fazer o .dot
    
    for(size_t id : idNos){
        arquivo_saida << endl << id; // linha do nó do momento
        for(Edge* aresta : arestas){

            if(aresta->_source_id == id){
                arquivo_saida << " -> " << aresta->_target_id;
                // if(this->_directed){
                //     arquivo_saida << " -> " << aresta->_target_id;
                //     // cout << " -> " << aresta->_target_id;
                // } else {
                //     arquivo_saida << " -- " << aresta->_target_id;
                //     // cout << " -- " << aresta->_target_id;
                // }   
            }

        }
    }
}


int Graph::getGrauNo(size_t node_id) {
    int grau = 0;
    for (Edge* aresta = search_for_node(node_id)->_first_edge; aresta != nullptr; aresta = aresta->_next_edge) {
        grau++;
    }
    return grau;
}

void Graph::caminho_prof_pontos_artc_nao_direcionado(size_t node_id, size_t parent_id, vector<size_t>& pontos_articulacao, vector<int>& discovery, vector<int>& low, vector<bool>& visited, vector<bool>& is_in_stack, stack<size_t>& stk, int& time) {
    Node* node = search_for_node(node_id);
    visited[node_id] = true;
    is_in_stack[node_id] = true;
    stk.push(node_id);

    discovery[node_id] = low[node_id] = ++time;
    int filhos = 0;

    for (Edge* aresta = node->_first_edge; aresta != nullptr; aresta = aresta->_next_edge) {
        size_t vizinho_id = aresta->_target_id;

        if (!visited[vizinho_id]) {
            filhos++;
            caminho_prof_pontos_artc_nao_direcionado(vizinho_id, node_id, pontos_articulacao, discovery, low, visited, is_in_stack, stk, time);

            // Atualiza o valor de low[node_id] com o menor valor alcançável a partir do vizinho
            low[node_id] = min(low[node_id], low[vizinho_id]);

            // Para grafos não direcionados e direcionados
            if ((parent_id == node_id && filhos > 1) || 
                (parent_id != node_id && low[vizinho_id] >= discovery[node_id])) {
                pontos_articulacao.push_back(node_id);
            }

        } else if (is_in_stack[vizinho_id] && vizinho_id != parent_id) {
            // Atualiza low[node_id] para considerar a aresta de retorno
            low[node_id] = min(low[node_id], discovery[vizinho_id]);
        }
    }
}

vector<size_t> Graph::getPontosArticulacaoNaoDirecionado() {
    vector<size_t> articulation_points;
    vector<int> discovery(this->_number_of_nodes + 1, -1);
    vector<int> low(this->_number_of_nodes + 1, -1);
    vector<bool> visited(this->_number_of_nodes + 1, false);
    vector<bool> is_in_stack(this->_number_of_nodes + 1, false);
    stack<size_t> stk;
    int time = 0;

    for (size_t i = 1; i <= this->_number_of_nodes; i++) {
        if (!visited[i]) {
            caminho_prof_pontos_artc_nao_direcionado(i, i, articulation_points, discovery, low, visited, is_in_stack, stk, time);
        }
    }

    sort(articulation_points.begin(), articulation_points.end());

    return articulation_points;
}

vector<size_t> Graph::getPontosArticulacaoDirecionados() {
    int time = 0;
    vector<int> discovery(_number_of_nodes, -1);
    vector<int> low(_number_of_nodes, -1);
    vector<bool> visited(_number_of_nodes, false);
    vector<size_t> pontos_articulacao;

    for (size_t i = 1; i <= _number_of_nodes; i++) {
        if (!visited[i]) {
            caminho_prof_pontos_art_direcionado(i, -1, pontos_articulacao, discovery, low, visited, time);
        }
    }

    // Remover duplicatas, se houver, e imprimir os pontos de articulação
    sort(pontos_articulacao.begin(), pontos_articulacao.end());
    pontos_articulacao.erase(unique(pontos_articulacao.begin(), pontos_articulacao.end()), pontos_articulacao.end());

    return pontos_articulacao;
}

void Graph::caminho_prof_pontos_art_direcionado(size_t node_id, size_t parent_id, vector<size_t>& pontos_articulacao, vector<int>& discovery, vector<int>& low, vector<bool>& visited, int& time) {
    visited[node_id] = true;
    discovery[node_id] = low[node_id] = ++time;
    int children = 0;

    for (Edge* aresta = search_for_node(node_id)->_first_edge; aresta != nullptr; aresta = aresta->_next_edge) {
        size_t vizinho_id = aresta->_target_id;

        if (!visited[vizinho_id]) {
            children++;
            caminho_prof_pontos_art_direcionado(vizinho_id, node_id, pontos_articulacao, discovery, low, visited, time);

            // Atualiza low[node_id] considerando o menor valor alcançável
            low[node_id] = min(low[node_id], low[vizinho_id]);

            // Condição de ponto de articulação em grafos direcionados
            if (parent_id != -1 && low[vizinho_id] >= discovery[node_id]) {
                if (getGrauNo(parent_id) != 2) { // Verifica se o grau do nó anterior é 2
                    pontos_articulacao.push_back(node_id);
                }
            }

            if (parent_id == -1 && children > 1) {
                pontos_articulacao.push_back(node_id);
            }
        } else if (vizinho_id != parent_id) {
            low[node_id] = min(low[node_id], discovery[vizinho_id]);
        }
    }
}
