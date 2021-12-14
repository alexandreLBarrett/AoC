#include "../helpers.h"
#include "../common.h"

#include <regex>

using namespace std;

class Molecule {
    string molecule;
public:
    Molecule(ifstream& stream) {
        getline(stream, molecule);
        stream.ignore(1);
    }

    vector<string> applyReplacements;
};

class Replacement {
    string lhs;
    string rhs;
public:
    Replacement(ifstream& stream) {
        getline(stream, lhs, ' ');
        stream.ignore(3);
        getline(stream, rhs);
    }
};

int main() {
    FileParser fp("2015/19-data-test");

    // Parse file
    auto molecule = fp.parseOne<Molecule>();
    auto replacements = fp.parseRest<Replacement>();

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