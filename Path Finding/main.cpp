#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <iterator>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

/*
Edge class. Roads between vertices.
*/
class Edge{
public:
    int from; //starting vertex.
    int to; //destination vertex.

    //constructs an edge.
    Edge(int f, int t){
        from = f;
        to = t;
    }
};
/*
Vertex class. Road intersections.
*/
class Vertex{
public:
    int id; //id of the vertex.
    int inDegree; //number of roads entering the vertex.
    int outDegree; //number of roads leaving the vertex.
    queue<Edge*> roads; //container for outgoing edges.

    //constructs a vertex.
    Vertex(int _id){
        id = _id;
        inDegree = 0;
        outDegree = INT_MAX;
    };

    //checks whether vertex is eularian or not.
    bool isEulerian(){
        return this->inDegree <= this->outDegree;
    }

};

/*
inserts a new circuit to an existing one.
*/
void merge(forward_list<int> &circuit, vector<int> &tour){
    int insertID = tour.at(tour.size()-1); //determines where the new circuit will be inserted to.
    
    forward_list<int>::iterator it = circuit.begin(); //iterator through existing circuit.
    for(it; it!=circuit.end(); it++)
        if(*it == insertID) break;
    
    circuit.insert_after(it, tour.begin(), tour.end());
}

/*
finds the next vertex with an unused edge.
*/
Vertex* findVertex(forward_list<int> &circuit, const vector<Vertex*> &graph){
    forward_list<int>::iterator it = circuit.begin(); //iterator through circuit.
    for(it; it!=circuit.end(); it++)
        if(graph.at(*it)->outDegree != 0) return graph.at(*it);
    
}

int main(int argc, char const *argv[]) {
    
    ifstream input(argv[1]); //input file.
    ofstream out(argv[2]); //output file.

    vector<Vertex*> graph; //container for vertices.

    int nofEgdes=0;
    int nofVertices; input>>nofVertices;

    graph.reserve(nofVertices); //sets specific size to container.
    
    //creates vertices with id.
    for(int i=0; i<nofVertices; i++){
        Vertex* vertex = new Vertex(i);
        graph.push_back(vertex);
    }

    for(int i=0; i<nofVertices; i++){ //assigns outDegree to vertices.
        int id, outDegree;
        input>>id; input>>outDegree;
        graph.at(id)->outDegree = outDegree;
        if(!graph.at(id)->isEulerian()){ //if vertex is not eularian, terminate.
            out<<"no path";
            return 0;
        } 

        for(int j=0; j<outDegree; j++){ //assigns neighbors to vertices and creates roads between them.
            int next_id; input>>next_id;
            Edge* edge = new Edge(id, next_id); nofEgdes++;
            graph.at(id)->roads.push(edge);
            graph.at(next_id)->inDegree++;
            if(!graph.at(next_id)->isEulerian()){ //if vertex is not eularian, terminate.
                out<<"no path";
                return 0;
            }
        }

    }
    
    forward_list<int> eulerianCircuit; //container for vertices which is going to be outputted.
    int size = 1;

    int startingVertex; input>>startingVertex; //vertex id to start circuit.
    Vertex* v = graph.at(startingVertex);

    eulerianCircuit.push_front(startingVertex);
        
    while(size <= nofEgdes){ //iterate until no edge left.
 
        vector<int> tour; //container for tour to be added to circuit.

        while(v->outDegree != 0){ //iterate if vertex has an unused edge.
            Edge* edge = v->roads.front(); v->roads.pop();
            v->outDegree--;
            graph.at(edge->to)->inDegree--;
            v = graph.at(edge->to);
            tour.push_back(v->id);
            delete (edge); //free heap
        }
        merge(eulerianCircuit, tour); //merge tour and circuit.
        size += tour.size(); //new size of circuit becomes size of the old one plus size of the tour.
        v = findVertex(eulerianCircuit, graph); //find next available vertex.
    
    }
    

    for(int i=0; i<graph.size(); i++){
    	delete graph.at(i); //free heap
    }
    
     
    //prints circuit to file.  
    for(int i=0; i<size; i++){ 
        out<<eulerianCircuit.front()<<" ";
        eulerianCircuit.pop_front();
    }

    return 0;
}