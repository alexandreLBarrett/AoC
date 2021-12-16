#include "../helpers.h"
#include "../common.h"

#include <regex>
#include <unordered_set>
#include <deque>

using namespace std;

class ReplacementSet {
public:
    unordered_map<string, vector<vector<string>>> set;

    ReplacementSet(ifstream& stream) {
        string lhs;
        string rhs;
        while(!stream.eof()) {
            getline(stream, lhs, ' ');
            stream.ignore(3);
            getline(stream, rhs);

            vector<string> elements;
            int index = 0;
            for (int i = 1; i < rhs.size(); i++) {
                if (isupper(rhs[i])) {
                    int offset = i - index;
                    elements.push_back(rhs.substr(index, offset));
                    index = i;
                }
            }
            elements.push_back(rhs.substr(index, rhs.size() - index));

            set[lhs].push_back(elements);
        }
    }
};

using Element = string;

class Molecule {
    vector<Element> moleculeVec;

    vector<Element> replaceMolecule(vector<Element> molecule, int index, int len, vector<Element> values) {
        molecule.erase(begin(molecule) + index, begin(molecule) + index + len);
        molecule.insert(begin(molecule) + index, begin(values), end(values));
        return molecule;
    }

    auto findSequenceIndex(const vector<Element>& molecule, const vector<Element>& searchFor, int offset = 0) {
        for (auto it = begin(molecule) + offset; it != end(molecule); it++) {
            auto copyIt = it;
            bool allGood = true;

            for (auto searchIt = begin(searchFor); searchIt != end(searchFor) && allGood; searchIt++, copyIt++) {
                allGood = *copyIt == *searchIt;
            }

            if (allGood)
                return it - begin(molecule);
        }
        return end(molecule) - begin(molecule);
    }
public:
    Molecule(ifstream& stream) {
        string molecule;
        getline(stream, molecule);
        stream.ignore(1);

        int index = 0;
        for (int i = 1; i < molecule.size(); i++) {
            if (isupper(molecule[i])) {
                int offset = i - index;
                moleculeVec.push_back(molecule.substr(index, offset));
                index = i;
            }
        }
        moleculeVec.push_back(molecule.substr(index, molecule.size() - index));
    }

    unordered_set<vector<Element>, VectorHash> getAllReplacements(const ReplacementSet& replacements) {
        unordered_set<vector<Element>, VectorHash> newMolecules;

        for (const auto& repl : replacements.set) {
            for (const auto& val : repl.second) {
                int index = find(begin(moleculeVec), end(moleculeVec), repl.first) - begin(moleculeVec);
                while (index < moleculeVec.size()) {
                    newMolecules.insert(replaceMolecule(moleculeVec, index, repl.first.size(), val));
                    index = find(begin(moleculeVec) + index + 1, end(moleculeVec), repl.first) - begin(moleculeVec);
                }
            }
        }

        return newMolecules;
    }

    bool getReverseReplacements(const ReplacementSet& replacements, deque<pair<int, vector<Element>>>& nextQueue) {
        const auto& molecule = nextQueue.front();

        for (const auto& repl : replacements.set) {
            for (const auto& val : repl.second) {
                int index = findSequenceIndex(molecule.second,  val);
                while (index < molecule.second.size()) {
                    auto replaced = replaceMolecule(molecule.second, index, val.size(), { repl.first });

                    if (replaced == vector<Element>{ "e" }) {
                        return true;
                    }

                    auto findEIt = find(begin(replaced), end(replaced), "e");
                    if (findEIt == end(replaced) || replaced.size() == 1) {
                        auto it = [&](){
                            auto first = nextQueue.begin();
                            auto last = nextQueue.end();
                            while (first!=last) {
                                if ((*first).second == replaced) return first;
                                ++first;
                            }
                            return last;
                        }();

                        if (it == nextQueue.end()) {
                            nextQueue.push_back(make_pair(molecule.first + 1, replaced));
                        }
                    }
                    
                    index = findSequenceIndex(molecule.second, val, index + 1);
                }
            }
        }

        return false;
    }

    int descendantAnalysis(const ReplacementSet& replacements) {
        deque<pair<int, vector<Element>>> compute;
        compute.push_back(make_pair(0, moleculeVec));
        bool found = false;

        pair<int, vector<Element>> molecule;
        do {
            molecule = compute.front();

            if (getReverseReplacements(replacements, compute)) {
                return molecule.first + 1;
            }

            compute.pop_front();
            sort(begin(compute), end(compute), [](const auto& a, const auto& b) {       
                return a.second.size() < b.second.size();
            });
        } while(molecule.second != vector<Element>{});

        return -1;
    }
};

int main() {
    FileParser fp("2015/19-data-test");

    // Parse file
    auto molecule = fp.parseOne<Molecule>();
    auto replacements = fp.parseOne<ReplacementSet>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto result = molecule.getAllReplacements(replacements).size();
        out = [=](auto& o) { o << result << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        auto result = molecule.descendantAnalysis(replacements);
        out = [=](auto& o) { o << result << endl; };
    });

    dph.RunAll(cout);
}
