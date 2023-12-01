#include "../helpers.h"
#include "../common.h"
#include "../graph.h"

#include <map>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/16

struct Valve {
    bool opened = false;
    string name;
    uint32_t flowRate = 0;
    vector<string> neighborValves = {};

    Valve() = default;
    Valve(const Valve& other) = default;
    Valve(istream& is) {
        string s;
        is >> s; // Valve
        is >> name;
        getline(is, s, '='); // has flow rate=
        is >> flowRate;

        is >> s; // ;
        is >> s; // tunnels
        is >> s; // lead
        is >> s; // to
        is >> s; // valves

        getline(is, s);

        string neighbor;
        for (int i = 1; i < s.length(); ++i) {
            if (s[i] == ',') {
                i += 2;
                neighborValves.push_back(neighbor);
                neighbor.clear();
            }
            neighbor += s[i];
        }
        neighborValves.push_back(neighbor);
    }
};

struct Volcano {
    Graph<string, Valve> valveGraph;

    map<string, map<string, vector<string>>> paths;

    Volcano(const vector<Valve>& newValves) {
        for (const auto& valve : newValves) {
            valveGraph.addNode(valve.name, valve);
            valveGraph.connectTo(valve.name, valve.neighborValves);
        }

        vector<string> potentialStart;
        for (const auto& [name, value] : valveGraph) {
            if (value.flowRate > 0) {
                potentialStart.push_back(name);
            }
        }

        for (const auto& name : potentialStart) {
            auto pathsTo = valveGraph.shortestPaths(name);
            for (const auto& [toName, path] : pathsTo) {
                paths[name][toName] = path;
            }
        }

        cout << potentialStart.size() << endl;
    }

    uint32_t getPressureReleaved() {
        uint32_t total = 0;
        for (auto it : valveGraph) {
            auto& [_, valve] = it;
            if (valve.opened)
                total += valve.flowRate;
        }
        return total;
    }
};

int main() {
    FileParser fp("2022/16-data-test");

    // Parse file
    auto valves = fp.parseRest<Valve>();
    
    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        Volcano volc(valves);

        out = [=](auto& o) {};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}