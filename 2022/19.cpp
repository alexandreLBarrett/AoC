#include "../helpers.h"
#include "../common.h"

#include <regex>
#include <map>
#include <array>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/19

/*
NOTE:
    Recursive brute force is too rough
*/

struct Blueprint {
    uint32_t costs[4][4] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
    };

    uint32_t toIndex(string type) {
        if (type == "ore") return 0;
        if (type == "clay") return 1;
        if (type == "obsidian") return 2;
        if (type == "geode") return 3;
        return -1;
    }

    bool build(array<uint32_t, 4>& materials, uint32_t robotType) {
        for (int i = 0; i < 4; ++i)
            if (materials[i] < costs[robotType][i])
                return false;

        for (int i = 0; i < 4; ++i)
            materials[i] -= costs[robotType][i];

        return true;
    }
    
    Blueprint(istream& is) {
        string s, statement;
        getline(is, s);

        regex rgx("Each (\\w+) robot costs ((?:(?:[0-9]+ \\w+)(?: and )?)+).");
        regex costsRgx("[0-9]+ \\w+");

        sregex_iterator next(s.begin(), s.end(), rgx), end;
        while (next != end) {
            smatch match = *next;
            string robotType = match[1].str();
            {
                string s = match[2].str();
                sregex_iterator next(s.begin(), s.end(), costsRgx), end;
                while (next != end) {
                    smatch match = *next;
                    string val = match[0].str();

                    int spacePos = val.find_first_of(' ');
                    string amount = val.substr(0, spacePos);
                    string type = val.substr(spacePos + 1);

                    costs[toIndex(robotType)][toIndex(type)] = stoi(amount);
                    ++next;
                }
            }
            ++next;
        }
    }
};

struct Fleet {
    Blueprint blueprint;

    // 0 = ore
    // 1 = clay
    // 2 = obsidian
    // 3 = geode

    Fleet(const Blueprint& bp) : blueprint(bp) {}

    void printState(array<uint32_t, 4> robots, array<uint32_t, 4> materials) {
        cout << "------------------------------------" << endl
            << "Robots: " << endl
            << "    ore: " << robots[0] << endl
            << "    clay: " << robots[1] << endl
            << "    obsidian: " << robots[2] << endl
            << "    geode: " << robots[3] << endl << endl
            << "Materials" << endl 
            << "    ore: " << materials[0] << endl
            << "    clay: " << materials[1] << endl
            << "    obsidian: " << materials[2] << endl
            << "    geode: " << materials[3] << endl << endl;
    }

    // recursive with input:
    //  materials[] : current materials counts
    //  robots[] : current robot counts
    // returns: 
    //  number of geodes


    uint32_t simulateForGeodes(uint32_t simulationTime
        , array<uint32_t, 4> robots
        , array<uint32_t, 4> materials
    ) {
        if (simulationTime == 0)
            return materials[3];

        //printState(robots, materials);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < robots[i]; ++j) {
                materials[i]++;
            }
        }

        uint32_t maxGeodes = 0;

        for (int i = 0; i < 4; ++i) {
            array<uint32_t, 4> newRobots = robots;
            array<uint32_t, 4> newMaterials = materials;
            if (blueprint.build(newMaterials, i)) {
                newRobots[i]++;
                maxGeodes = max(maxGeodes, simulateForGeodes(simulationTime - 1, newRobots, newMaterials));
            }
        }

        return max(maxGeodes, simulateForGeodes(simulationTime - 1, robots, materials));
    }
};


int main() {
    FileParser fp("2022/19-data-test");

    // Parse file
    auto blueprints = fp.parseRest<Blueprint>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        for (auto blueprint : blueprints) {
            Fleet fleet(blueprint);
            auto maxGeodes = fleet.simulateForGeodes(24, {1,0,0,0}, {0,0,0,0});
            cout << maxGeodes << endl;
        }

        out = [=](auto& o) {};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}