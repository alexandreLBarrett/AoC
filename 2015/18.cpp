#include "../helpers.h"
#include "../common.h"

using namespace std;

struct Point {
    int x, y;
};

class LightGrid {
    vector<bool> lights;
    int sideLen;

    bool check(int index, int offset) {
        if (!(
            index + offset >= 0 
            && index + offset < lights.size() 
            && (
                index % sideLen + offset >= 0
                && index % sideLen + offset <= sideLen
            )
        ))
        {
            int e = 0;
            e++;
        }

        return index + offset >= 0 
            && index + offset < lights.size() 
            && (
                index % sideLen + offset >= 0
                && index % sideLen + offset < sideLen
            );
    }

    vector<bool> getSurroundings(int i) {
        return {
            check(i - sideLen, -1) && lights[i - sideLen - 1],
            check(i - sideLen, 0)  && lights[i - sideLen],
            check(i - sideLen, 1)  && lights[i - sideLen + 1],
            check(i, -1)           && lights[i - 1],
            check(i, 1)            && lights[i + 1],
            check(i + sideLen, -1) && lights[i + sideLen - 1],
            check(i + sideLen, 0)  && lights[i + sideLen],
            check(i + sideLen, 1)  && lights[i + sideLen + 1],
        };
    }
public: 
    LightGrid(ifstream& stream) {
        for (istreambuf_iterator<char> it(stream), end; it != end; it++) {
            if (*it == '\n') continue;

            lights.push_back(*it == '#');
        }

        sideLen = sqrt(lights.size());

        lights[0] = true;
        lights[sideLen - 1] = true;
        lights[lights.size() - 1] = true;
        lights[lights.size() - sideLen] = true;
    }

    void calculateNext2() {
        vector<bool> nextSet = lights;
        for (int i = 0; i < lights.size(); i++) {
            auto surroundingValues = getSurroundings(i);
            int lightsOn = count(begin(surroundingValues), end(surroundingValues), true);

            if (
                i == 0 
                || i == sideLen - 1 
                || i == lights.size() - 1
                || i == lights.size() - sideLen
            ) {
                nextSet[i] = true;
            } else {
                nextSet[i] = lights[i] 
                    ? lightsOn == 2 || lightsOn == 3
                    : lightsOn == 3;
            }
        }

        lights = move(nextSet);
    }

    void calculateNext() {
        vector<bool> nextSet = lights;
        for (int i = 0; i < lights.size(); i++) {
            auto surroundingValues = getSurroundings(i);
            int lightsOn = count(begin(surroundingValues), end(surroundingValues), true);

            nextSet[i] = lights[i]
                ? lightsOn == 2 || lightsOn == 3
                : lightsOn == 3;
        }

        lights = move(nextSet);
    }

    void draw(ostream& o) const {
        for (int i = 0; i < lights.size(); i++) {
            o << (lights[i] ? '#' : '.');

            if ((i + 1) % sideLen == 0) {
                o << endl;
            }
        }
        o << "There are " << countLightsOn() << " lights on" << endl;
    }

    int countLightsOn() const {
        return count(begin(lights), end(lights), true);
    }
};


int main() {
    FileParser fp("2015/18-data");

    auto lightGrid = fp.parseOne<LightGrid>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        for (int i = 0; i < 100; i++) {
            lightGrid.calculateNext();
        }
        out = [=](auto& o) { lightGrid.draw(o); };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        for (int i = 0; i < 100; i++) {
            lightGrid.calculateNext2();
        }
        out = [=](auto& o) { lightGrid.draw(o);};
    });


    dph.RunAll(cout);
}