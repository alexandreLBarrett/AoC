#include "../helpers.h"
#include "../common.h"

#include <map>
#include <set>

using namespace std;

class Caves {
    map<string, vector<string>> paths;
    set<vector<string>> exploredCaves;

    bool isLower(const string& v) {
        for (const auto c : v) {
            if (!islower(c))
                return false;
        }

        return true;
    }

    template<class T>
    bool vectorContains(const vector<T>& vec, const T& val) {
        for (const auto& t : vec) {
            if (t == val)
                return true;
        }
        return false;
    }

    vector<string> getSmallCaves() {
        vector<string> ret;

        for (const auto& path : paths) {
            if (path.first == "start" || path.first == "end")
                continue;

            if (!isLower(path.first))
                continue;

            ret.push_back(path.first);
        }   

        return ret;
    }

    void exploreCave(const string& caveName, vector<string> currentPath, vector<string> exploredSmallCaves, const string& doubleSmall) {
        currentPath.push_back(caveName);

        if (caveName == "end") {
            exploredCaves.insert(currentPath);
            return;
        }

        if (isLower(caveName))
            exploredSmallCaves.push_back(caveName);

        vector<string> toExplore = paths[caveName];
        for (const auto& cave : toExplore) {
            
            if (cave == doubleSmall 
                ? count(begin(exploredSmallCaves), end(exploredSmallCaves), cave) > 1
                : count(begin(exploredSmallCaves), end(exploredSmallCaves), cave) > 0
            )
                continue;

            exploreCave(cave, currentPath, exploredSmallCaves, doubleSmall);
        }
    }

public: 
    Caves(ifstream& stream) {
        while(!stream.eof()) {
            string key, val;
            getline(stream, key, '-');
            getline(stream, val);
            paths[key].push_back(val);
            paths[val].push_back(key);
        }
    }

    set<vector<string>> getUniquePaths() {
        exploreCave("start", {}, {}, "");

        return exploredCaves;
    }

 
    set<vector<string>> getUniquePaths2() {
        auto smallCaves = getSmallCaves();
        
        for (const auto& cave : smallCaves) {
            exploreCave("start", {}, {}, cave);
        }

        return exploredCaves;
    }
};

int main() {
    FileParser fp("2021/12-data");
    auto caves = fp.parseOne<Caves>();

    DayPartHandler dph;

    dph.AddPart([=](auto& out) mutable {
        int caveCount = caves.getUniquePaths().size();
        out = [=](auto& o) { o << caveCount; };
    });

    dph.AddPart([=](auto& out) mutable {
        int caveCount = caves.getUniquePaths2().size();
        out = [=](auto& o) { o << caveCount; };
    });

    dph.RunAll(cout);
}
