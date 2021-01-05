#include <vector>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;


ostream& operator <<(ostream& op, vector<int>a){
    cout << "[";
    for(int i = 0; i<a.size(); i++){
        cout << a[i];
        if(i == a.size()-1)
        continue;
        cout << ", ";
    }
    cout << "]\n";
    return op;
}
ostream& operator <<(ostream& op, vector<pair<int,int>>a){
    cout << "{";
    for(int i = 0; i<a.size(); i++){
        cout << "{" << a[i].first << ", " << a[i].second << "}\n";
    }
    cout << "}";
    return op;
}


class Graph{
    private:     
    public:
    vector<vector<int>> graph;
    vector<vector<int>> weight;
    vector<pair<int, int>> edges;
        int vertices;
        void readAdjacencyMatrix(int numOfVertices, vector<vector<int>> matrix){
            vertices = numOfVertices;
            for(int i = 0; i<numOfVertices; i++){
                vector<int>edge;
                for(int j = 0; j < numOfVertices; j++){
                    if(matrix[i][j] == 1){
                        edge.push_back(j);
                        edges.push_back(make_pair(i, j));
                    }
                }
                weight.push_back(vector<int>(numOfVertices, 0));
                graph.push_back(edge);
            }
        }
        void readAdjacencyList(int numOfVertices, vector<vector<int>> matrix){
            vertices = numOfVertices;
            for(int i = 0; i<numOfVertices; i++){
                graph.push_back(matrix[i]);
                weight.push_back(vector<int>(numOfVertices, 0));
                for(int j : matrix[i])
                edges.push_back(make_pair(i, j));
            }
        }
        bool isDirected(){
            vector<vector<int>> adjMatrix(vertices, vector<int>(vertices, 0));
            for(int u = 0; u < vertices; u++){
                for(int v : graph[u]){
                    adjMatrix[u][v] =1;
                }
            }
            for(int i = 0; i < vertices; i++){
                for(int j = 0; j < vertices; j++){
                    if(adjMatrix[i][j] != adjMatrix[j][i])
                    return true;
                }
            }
            return false;
        }
        int numberOfEdges(){
            int edges =0;
            for(int i = 0; i < vertices; i++){
                edges += graph[i].size();
            }
            return edges;
        }
        int numberOfVertices(){
            return vertices;
        }
        vector<vector<int>> degreeOfVertices(){
            bool isDirectedGraph = isDirected();
            if(!isDirectedGraph){
                vector<vector<int>> degrees(vertices, vector<int>(1, 0));
                for(int i = 0; i < vertices; i++){
                    degrees[i][0] += graph[i].size();
                }
                return degrees;
            }

            // undirected graph;
            // degrees[i][0] -- in
            // degrees[i][1] -- out
            vector<vector<int>> degrees(vertices, vector<int>(2, 0));
            for(int u = 0; u < vertices; u++){
                for(int v : graph[u]){
                    degrees[u][1] += 1;
                    degrees[v][0] += 1;
                }
            }
            return degrees;
        }
        vector<int> isolatedVertices(){
            vector<int>list;
            vector<vector<int>> degrees = degreeOfVertices();
            for(int u = 0; u<vertices; u++){
                bool flag = true;
                for(int i =0; i<degrees[u].size();i++){
                    if(degrees[u][i] != 0) {flag = false; break;}
                }
                if(flag) list.push_back(u);
            }
            return list;
        }
        bool isCompleteGraph(){
            return numberOfEdges() == ((vertices-1)*vertices)/2;
        }
        bool isCircularGraph(){
            vector<vector<int>> degrees = degreeOfVertices();
            for(int u = 0; u<vertices; u++){
                if(degrees[u][0] != 2) return false;
            }
            return true;
        }
        bool dfsCheckBigraph(int u, vector<int>& color){
            for(int v : graph[u])  {
                if(color[v] == -1){
                    color[v] == 1 - color[u];
                    dfsCheckBigraph(v, color);
                }else if(color[v] != 1-color[u]) return false;
            }
            return true;
        }
        bool isBigraph(){
            if(vertices < 1) return false;
            vector<int>color(vertices, -1);
            bool isTrue = false;
            for(int u = 0; u<vertices; u++){
                if(color[u] == -1){
                    color[u] = 0;
                    isTrue |= dfsCheckBigraph(u, color);
                }
            }
            return isTrue;
        }
        bool isCompleteBigraph(){
            // return isBigraph() && 
            return true;
        }
        bool dfsCheckCycle(int u, vector<int>& visited){  
            visited[u] = 1;
            bool isCycle = false;
            for(int v:graph[u]){
                if(visited[v] == 0) {
                    isCycle |= dfsCheckCycle(v, visited);
                }else if(visited[v] == 1){
                    return true;
                }
            }
            visited[u] = 2;
            return isCycle;
            // O(m+v) -- directed graph
        }
        bool isCyclic(){
            bool isCycle = false;
            vector<int> visited(vertices, 0);
            for(int i = 0; i<vertices; i++){
                if(visited[i] == 0){
                    bool res = dfsCheckCycle(i, visited);
                    isCycle |= res;
                }
            }
            return isCycle;
        }
        void DFS(int u, vector<bool>&visited, vector<vector<int>> G){
            visited[u] == true;
            for(int v: G[u]){
                if(!visited[v]){
                    DFS(v, visited, G);
                }
            }
        }
        int numberOfComponents(vector<vector<int>> G){
            vector<bool>visited(vertices, false);
            int count =0 ;
            for(int u = 0; u < vertices; u++){
                if(!visited[u]){
                    DFS(u, visited, G);
                    count++;
                }
            }
            return count;
        }
        
        int numberOfCutVertices(){
            int originalComponents = numberOfComponents(this->graph);
            int count = 0;
            for(int u = 0; u < vertices; u++){
                vector<vector<int>> GTmp(this->graph);
                GTmp[u].clear();
                int components = numberOfComponents(GTmp);
                if(components > originalComponents){
                    count ++;
                }
            }
            return count;
        }
        vector<vector<int>> AdjacencyMatrix(){
            vector<vector<int>>matrix(vertices, vector<int>(vertices, 0));
            for(int u = 0; u <vertices; u++){
                for(int v:graph[u]){
                    matrix[u][v] = 1;
                }
            }
            return matrix;
        }
        vector<vector<int>> BaseUndirectedGraph(){
            vector<vector<int>>matrix(vertices, vector<int>(vertices, 0));
            for(int u = 0; u <vertices; u++){
                for(int v:graph[u]){
                    matrix[u][v] = 1;
                    matrix[v][u] = 1;
                }
            }
            return matrix;
        }
        vector<vector<int>> ComplementGraph(){
            vector<vector<int>> matrix = AdjacencyMatrix();
            for(int i = 0; i<vertices; i++){
                for(int j = 0; j<vertices; j++){
                    if(i == j) continue;
                    matrix[i][j] = 1- matrix[i][j];
                }
            }
            return matrix;
        }
        vector<vector<int>> ConverseGraph(){
            vector<vector<int>> matrix = AdjacencyMatrix();
            for(int i = 0; i<vertices; i++){
                for(int j = 0; j<vertices; j++){
                    if(matrix[i][j]==1){ 
                        matrix[j][i] = 1;
                        matrix[i][j] = 0;
                    }
                }
            }
            return matrix;
        }

        void HierholzersDFS(int u, vector<int>& edges, vector<int>& cycle){
            if(edges[u] > 0){
                int tmp = --edges[u];
                HierholzersDFS(graph[u][tmp], edges, cycle);
            }
            cycle.push_back(u);
        }
        vector<int> EulerCycle(){
            vector<vector<int>> degrees = degreeOfVertices();
            
            for(int i = 0; i < vertices; i++){
                int deg = 0;
                for(int d : degrees[i]) deg += d;
                if(deg%2 != 0) return {};
            }
            vector<int> edges(vertices, 0);
            bool is_directed = isDirected();
            for(int i = 0; i<vertices; i++){
                if(is_directed){
                    edges[i] = degrees[i][0];
                }else{
                    edges[i] = degrees[i][1];
                }
            }
            vector<int> cycle;
            HierholzersDFS(0, edges, cycle);
            reverse(cycle.begin(), cycle.end());
            return cycle;
        }


        // Minimum spanning tree
        // Disjoin set
        int root(int v, vector<int>parents){
            while(parents[v] != -1){
                v = parents[v];
            }
            return v;
        }
        void unionSet(int a, int b, vector<int>&parents){
            int rootA = root(a, parents);
            int rootB = root(b, parents);
            parents[rootB] = rootA;
        }
        bool MakingCycle(vector<int>&parents, pair<int, int> edges){
            int rootA = root(edges.first, parents);
            int rootB = root(edges.second, parents);
            if(rootA != rootB){
                unionSet(rootA, rootB, parents);
                return false;
            }
            return true;
        }
        bool compareInterval(pair<int, int>* e1, pair<int, int>* e2)
        {
            return (weight[e1->first][e1->second] > weight[e2->first][e2->second]);
        }
        vector<pair<int,int>> MinimumSpanningTree(){
            bool isDirectedG = isDirected();
            for(int i = 0; i<edges.size()-1; i++){
                for(int j = i+1; j < edges.size(); j++)
                if(weight[edges[i].first][edges[i].second]> weight[edges[j].first][edges[j].second]){
                    swap(edges[i], edges[j]);
                }
            }
            int rootOfTree = -1;
            vector<int>parents(vertices, -1);
            cout << parents << endl;
            vector<pair<int, int>> res;
            for(pair<int, int> edge : edges){
                if(!isDirectedG && edge.first > edge.second) continue;
                if(!MakingCycle(parents, edge)){
                    res.push_back(edge);
                };
                cout << parents;
            }
            return res;
        }
};
