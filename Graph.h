#include "Node.h"

#include <map>

class Graph {

private:

    std::map<int, Node> nodes;

public:

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++)
            nodes[nodeID] = Node();
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

};