#pragma once

#include "Node.h"

#include <map>
#include <stack>
#include <queue>

class Graph {

private:

    std::map<int, Node> nodes;

public:

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
    }

    bool isDependent(int index, int node) {
        bool isDependent = false;
        for (auto& node_temp : nodes[index].getNodes()) {
            if (node_temp == node) {
                isDependent = true;
            }
        }
        return isDependent;
    }

    //toString class
    std::string toString() {
        std::stringstream s;

        for (auto& pair: nodes) {
            int nodeID = pair.first;
            Node node = pair.second;
            s << "R" << nodeID <<":" << node.toString() << "\n";
        }
        return s.str().substr(0, s.str().size() - 1);
    }


    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }
    Graph reverseGraph() {
        Graph rgraph(this->nodes.size());
        for (auto& node : nodes) {
            for (const auto& anode : node.second.getNodes()) {
                rgraph.addEdge(anode, node.first);
            }
        }
        return rgraph;
    }

    std::stack<int> dfsForest() {
        std::stack<int> rnode;
        std::stack<int> adjNodes;
        std::stack<int> dfs;
        std::queue<int> tovisit;
        std::map<int, bool> visited;
        int node = 0;
        for (unsigned int i = 0; i < this->nodes.size(); i++) {
            visited[i] = false;
        }
        while (dfs.size() < this->nodes.size()) {
            visited[node] = true;
            rnode.push(node);
            for (const int adj : this->nodes[node].getNodes()) {


                if (!visited[adj]) {
                    visited[adj] = true;
                    adjNodes.push(adj);
                }
            }
            while (!adjNodes.empty()) {
                tovisit.push(adjNodes.top());
                adjNodes.pop();
            }
            if (tovisit.empty()) {
                for (auto& visit : visited) {
                    while (!rnode.empty()) {
                        dfs.push(rnode.top());
                        rnode.pop();
                    }
                    if (!visit.second) {
                        node = visit.first;
                        break;
                    }
                }
            }
            else {
                node = tovisit.front();
                tovisit.pop();
            }
        }
        
        return dfs;
    }
    

    std::vector<std::vector<int>> findSCC() {
        Graph rgraph = this->reverseGraph();
        std::stack<int> order = rgraph.dfsForest();
        std::map<int, bool> visited;
        
        std::vector<std::vector<int>> sccs;

        for (unsigned int i = 0; i < this->nodes.size(); i++) {
            visited[i] = false;
        }
        
        int t_node;
        std::set<int> values;
        std::queue<int> tovisit;
        std::stack<int> adjNodes;
        while (!order.empty()){
            values.clear();
            if (!visited[order.top()]) {
                tovisit.push(order.top());
            
            values.insert(tovisit.front());

            while (!tovisit.empty()) {
                t_node = tovisit.front();
                tovisit.pop();
                visited[t_node] = true;
                for (const int adj : this->nodes[t_node].getNodes()) {
                    if (!visited[adj]) {
                        values.insert(adj);
                        visited[adj] = true;
                        tovisit.push(adj);
                    }
                }
            }
            std::vector<int> scc(values.begin(), values.end());
            sccs.push_back(scc);
            }
            order.pop();

        }
        return sccs;

        
    }

};