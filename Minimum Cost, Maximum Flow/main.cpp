#include <vector>
#include <fstream>
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

typedef pair<int,int> Edge; //names pair of integers as Edge.
int lastUpdatedVertex = -1; //stores last updated vertex's id.

/*
detects whether there is negative cycle.
code retrieved from https://konaeakira.github.io/posts/using-the-shortest-path-faster-algorithm-to-find-negative-cycles.html
@param pre : predecessors of vertices.
@param n : matrix size.
*/
bool detectCycle(vector<int>& pre, int n){
    vector<int> vec;
    vector<bool> onstack(2*n, false);
    vector<bool> visited(2*n, false);

    for(int i=0; i<2*n; i++){
        if(!visited.at(i)){
            for(int j = i; j != -1; j = pre.at(j)){
                if(!visited.at(j)){
                    visited.at(j) = true;
                    vec.push_back(j);
                    onstack.at(j) = true;
                }
                else{
                    if(onstack.at(j)){
                        lastUpdatedVertex = j; //updates last updated vertex.
                        return true;
                    }
                    break;
                }
            }
            for(int j: vec){
                onstack.at(j) = false;
            }
            vec.clear();
        }

    }
    return false;
}

/*
finds negative cycle and returns it as a stack.
code retrieved from ps12, https://github.com/rizaozcelik/cmpe250-fall20/blob/main/PS12/bellman_ford.cpp
@param pre : predecessors of vertices.
@param lastUpdatedVertex : last updated vertex id.
*/
stack<int> trace(vector<int>& pre, int lastUpdatedVertex){
    stack<int> cycle; 
    int parent = pre.at(lastUpdatedVertex); //parent of last updated vertex.

    cycle.push(lastUpdatedVertex);
    while(lastUpdatedVertex != parent){ //keep adding until coming back where started.
        cycle.push(parent);
        parent = pre.at(parent);
    }
    cycle.push(lastUpdatedVertex);

    return cycle;
}
/*
updates the graph with found negative cycle.
@param graph : graph to be updated.
@param flow : final voltage storage.
@param cycle : negative cycle stack.
*/
void update(vector<Edge> graph[], vector<Edge> flow[], stack<int>& cycle){
    int x = cycle.top(); cycle.pop(); //first element of cycle.

    while(!cycle.empty()){
        int y = cycle.top(); cycle.pop(); //next element of cycle.
        
        int index = 0;
        int new_cost; //final voltage value.
        /*
        find edge to be updated and add new edge, erase unnecessary one.
        */
        for(Edge residual: graph[x]){
            if(residual.first == y){
                graph[y].emplace_back(x, residual.second * -1);
                new_cost = residual.second * -1;
                graph[x].erase(graph[x].begin()+index);
                break;
            }
            index++;
        }
        /*
        find voltage source to be updated and update.
        */
        for(Edge final: flow[x]){
            if(final.first != y){
                flow[x].emplace_back(y, new_cost);
                flow[x].erase(flow[x].begin());
                break;
            }
        }
        x = y;
    }

}

/*
code retrieved from https://konaeakira.github.io/posts/using-the-shortest-path-faster-algorithm-to-find-negative-cycles.html
@param graph : graph to be worked on.
@param flow : final voltage storage.
@param n : matrix size.
*/
bool spfa(vector<Edge> graph[], vector<Edge> flow[], int n){
    vector<int> distances; distances.reserve(2*n);
    vector<int> pre; pre.reserve(2*n);
    vector<bool> inq; inq.reserve(2*n);
    queue<int> que;
    for(int i=0; i<2*n; i++){
        distances.push_back(0);
        pre.push_back(-1);
        inq.push_back(true);
        que.push(i);
    }

    int iter = 0;
    while(!que.empty()){

        int u = que.front(); que.pop();
        inq.at(u) = false;

        for(Edge edge: graph[u]){ //traverse all edges going from "u".
            if(distances.at(u) + edge.second < distances.at(edge.first)){
                
                pre.at(edge.first) = u;
                distances.at(edge.first) = distances.at(u) + edge.second;

                if(++iter == n*2){
                    iter = 0;
                    if(detectCycle(pre, n)){ //if there is  a negative cycle,
                        stack<int> cycle = trace(pre, lastUpdatedVertex); //find the cycle,
                        update(graph, flow, cycle); //then update the graph.
                        return true;
                    }
                }
                if(!inq.at(edge.first)){
                    que.push(edge.first);
                    inq.at(edge.first) = true;
                }
            }
        }
    }
    if(detectCycle(pre, n)){
        stack<int> cycle = trace(pre, lastUpdatedVertex);
        update(graph, flow, cycle);
        return true;
    }
    return false;
}

/*
driver method
*/
int main(int argc, char* argv[]) {

    ifstream input(argv[1]); //input file.
    ofstream out(argv[2]); //output file.

    int nofTestCases; input>>nofTestCases;

    for(int u=0; u<nofTestCases; u++) { //traverse for all test cases in the input.

        int n; input>>n;

        vector<Edge> flow[2*n]; //final voltage storage.
        vector<Edge> graph[2*n]; //graph to work with.

        /*
        construct the graph.
        */
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){

                int cost; input>>cost;

                if(i==j){
                    flow[i].emplace_back(i+n, cost);
                    graph[i+n].emplace_back(i, cost);
                }else{
                    graph[i].emplace_back(j+n, -1*cost);
                }
            }
        }

        bool tf = spfa(graph, flow, n);
        while(tf){
            tf = spfa(graph, flow, n);
        }

        int total = 0;
        for(int i=0; i<n; i++){
            total += flow[i].at(0).second;
        }
        out<<total<<endl; //write to the output.


    }
    
    return 0;
}
