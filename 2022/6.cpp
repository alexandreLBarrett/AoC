#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/6

class ElfStream {
    string s;
public:

    ElfStream(istream& is) {
        
    }
};

int main() {
    FileParser fp("2022/6-data-test");

    // Parse file

    DayPartHandler<ostream> dph;

    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}