#include "../helpers.h"
#include "../common.h"
#include "../string_manip.h"

#include <regex>
#include <map>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/19

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
    uint32_t materials[4] = { 0, 0, 0, };
    uint32_t robots[4] = { 1, 0, 0, 0 };

    Fleet(const Blueprint& bp) : blueprint(bp) {}

    void printState() {
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

    void simulate(uint32_t simulationTime) {
        while (simulationTime != 0) {
            // Decide whether to build a robot or not

            // Each robot harvests that turn
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < robots[i]; ++j) {
                    materials[i]++;
                }
            }

            printState();

            // A minute passes
            --simulationTime;
        }
    }

    uint32_t getGeodeCount() {
        return materials[3];
    }
};


int main() {
    FileParser fp("2022/19-data-test");

    // Parse file
    auto blueprints = fp.parseRest<Blueprint>();

    Fleet fleet(blueprints[0]);
    fleet.simulate(24);

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        
        out = [=](auto& o) {};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}