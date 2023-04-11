#pragma once

class Node {

private:

    std::set<int> adjacentNodeIDs;

public:

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }
    std::string toString() const{
        std::stringstream s;
        for (const int& node : adjacentNodeIDs) {
            s << "R" << node << ",";
        }
        s << "";
        return s.str().substr(0, s.str().size() - 1);
    }
    std::set<int> getNodes() const{
        return this->adjacentNodeIDs;
    }

};