#pragma once
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <optional>

template<class T, class V>
class Graph {
    std::map<T, V> values;
    std::map<T, std::set<T>> connections;

    std::string error;

    std::map<T, uint64_t> dijkstraDistances(const T& start) const;
public:
    bool addNode(const T& nodeName, const V& value);
    std::optional<V> getNode(const T& nodeName);
    bool removeNode(const T& nodeName);
    const std::set<T>& neighbors(const T& nodeName) const;
    bool connectTo(const T& nodeName, const std::vector<T>& nodes);
    bool detachFrom(const T& nodeName, const std::vector<T>& nodes);

    std::string getError() const { return error; };
    std::map<T,V>::iterator begin();
    std::map<T,V>::const_iterator cbegin() const;
    std::map<T,V>::iterator end();
    std::map<T,V>::const_iterator cend() const;

    // Gets shortestPath from startingNode to all other using dijkstra's algo
    std::map<T, std::vector<T>> shortestPaths(const T& startingNode) const;
    std::vector<T> shortestPath(const T& startingNode, const T& endNode) const;
};

template<class T, class V>
bool Graph<T,V>::addNode(const T& nodeName, const V& value) {
    if (values.contains(nodeName)) {
        error = "Node of that name already exist";
        return false;
    }
    values[nodeName] = value;
    connections[nodeName];
    return true;
}

template<class T, class V>
bool Graph<T,V>::connectTo(const T& nodeName, const std::vector<T>& nodes) { 
    if (!values.contains(nodeName)) {
        error = "Node doesnt exist";
        return false;
    }

    for (const auto& node : nodes)
        connections[nodeName].insert(node);
    return true;
}

template<class T, class V>
bool Graph<T,V>::detachFrom(const T& nodeName, const std::vector<T>& nodes) {
    if (!values.contains(nodeName)) {
        error = "Node doesnt exist";
        return false;
    }

    for (const auto& node : nodes)
        connections[nodeName].erase(node);
    return true;
}

template<class T, class V>
const std::set<T>& Graph<T,V>::neighbors(const T& nodeName) const {
    return connections[nodeName];
}

template<class T, class V>
std::map<T,V>::iterator Graph<T,V>::begin() {
    return values.begin();
};

template<class T, class V>
std::map<T,V>::iterator Graph<T,V>::end() {
    return values.end();
};

template<class T, class V>
std::map<T,V>::const_iterator Graph<T,V>::cbegin() const {
    return values.cbegin();
};

template<class T, class V>
std::map<T,V>::const_iterator Graph<T,V>::cend() const {
    return values.cend();
};

template<class T, class V>
std::optional<V> Graph<T,V>::getNode(const T& nodeName) {
    if (!values.contains(nodeName)) {
        error = "Node doesnt exist";
        return std::nullopt_t;
    }

    return make_optional(values[nodeName]);
}

template<class T, class V>
bool Graph<T,V>::removeNode(const T& nodeName) {
    if (!values.contains(nodeName)) {
        error = "Node doesnt exist";
        return false;
    }

    values.erase(nodeName);
    connections.erase(nodeName);

    return true;
}

template<class T, class V>
std::map<T, uint64_t> Graph<T,V>::dijkstraDistances(const T& startingNode) const {
    using namespace std;

    set<T> unvisited;
    map<T, uint64_t> distances;

    for (const auto& [name, value] : values) {
        unvisited.insert(name);
        if (name == startingNode)
            distances[name] = 0;
        else
            distances[name] = UINT64_MAX;
    }

    T current = startingNode;
    while (unvisited.size() > 0) {
        set<T> neighbors = connections.at(current);
        for (const auto& node : neighbors) {
            distances[node] = min(distances[current] + 1, distances[node]);
        }

        unvisited.erase(current);

        T closest;
        uint64_t maxDist = UINT64_MAX;
        for (const T& unv : unvisited) {
            uint64_t dist = distances[unv];
            if (dist < maxDist) {
                maxDist = dist;
                closest = unv;
            }
        }

        current = closest;
    }

    return distances;
}

template<class T, class V>
std::map<T, std::vector<T>> Graph<T, V>::shortestPaths(const T& startingNode) const {
    const auto& distances = dijkstraDistances(startingNode);

    std::map<T, vector<T>> dists;
    for (const auto& [name, val] : values) {
        dists[name] = {};

        T current = name;
        while (current != startingNode) {
            uint64_t dist = distances.at(current);
            for (const T& conn : connections.at(current)) {
                if (distances.at(conn) == dist - 1) {
                    current = conn;
                    dists[name].push_back(conn);
                    break;
                }
            }
        }
    }

    return dists;
}

template<class T, class V>
std::vector<T> Graph<T, V>::shortestPath(const T& startingNode, const T& endNode) const {
    const auto& distances = dijkstraDistances(startingNode);

    std::vector<T> path;
    T current = endNode;
    while (current != startingNode) {
        uint64_t dist = distances[current];
        for (const T& conn : connections[current]) {
            if (distances[conn] == dist - 1) {
                current = conn;
                path.push_back(conn);
                break;
            }
        }
    }

    return path;
}