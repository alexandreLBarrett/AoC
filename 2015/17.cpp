#include "../helpers.h"
#include "../common.h"

#include <map>
#include <numeric>
#include <deque>
#include <set>

using namespace std;

const size_t ONCES_TO_FIT = 150;

class EggnogContainers {
    deque<size_t*> containers;
    map<deque<size_t*>, bool> paths;

    void getCombinations(deque<size_t*> currentPath, size_t* next, const deque<size_t*>& remainingContainers, size_t total) {
        currentPath.push_back(next);
        total += *next;

        sort(currentPath.begin(), currentPath.end());

        size_t sum = accumulate(remainingContainers.begin(), remainingContainers.end(), 0, [](size_t v, size_t* op) {
            return v + *op;
        });

        if (total + sum < ONCES_TO_FIT)
            return;
    
        if (total == ONCES_TO_FIT) {
            paths[currentPath] = true;
            return;
        }

        if (total > ONCES_TO_FIT)
            return;

        deque<size_t*> remainders = remainingContainers;
        for (auto cont : remainingContainers) {
            remainders.pop_front();
            getCombinations(currentPath, cont, remainders, total);
        }
    }
public:
    EggnogContainers(ifstream& stream) {
        while (!stream.eof()) {
            size_t* value = new size_t;
            stream >> *value;
            containers.push_back(value);
        }

        sort(containers.begin(), containers.end(), [](const size_t* a, const size_t* b) { return *a > *b; });
    }

    EggnogContainers(const EggnogContainers& other) {
        for (auto val : other.containers) {
            size_t* ptrVal = new size_t;
            *ptrVal = *val;
            containers.push_back(ptrVal);
        }
        sort(containers.begin(), containers.end(), [](const size_t* a, const size_t* b) { return *a > *b; });
    }

    size_t getNumberOfCombinations() {
        deque<size_t*> remainders = containers;
        for (auto cont : containers) {
            remainders.pop_front();
            getCombinations({}, cont, remainders, 0);
        }

        return paths.size();
    }

    vector<deque<size_t*>> getMinContainers() {
        vector<deque<size_t*>> retVal;
        int minSize = INT_MAX;

        for (auto v : paths) {
            int size = v.first.size();
            if (size < minSize) {
                retVal.clear();
                minSize = size;
            }

            if (minSize == size) {
                retVal.push_back(v.first);
            }
        }

        return retVal;
    }

    ~EggnogContainers() {
        for (auto values : containers) {
            delete values;
        }
        containers.clear();
    }
};

int main() {
    FileParser fp("2015/17-data");

    auto eggnogs = fp.parseOne<EggnogContainers>();

    DayPartHandler dph;

    dph.AddPart([=](auto& out) mutable {
        auto sol = eggnogs.getNumberOfCombinations();

        out = [=](auto& o) { o << sol; };
    });

    dph.AddPart([=](auto& out) mutable {
        auto sol = eggnogs.getNumberOfCombinations();

        auto conts = eggnogs.getMinContainers();
    
        out = [=](auto& o) { o << conts.size(); };
    });

    dph.RunAll(cout);
}
