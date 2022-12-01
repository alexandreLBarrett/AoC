#include "../helpers.h"
#include "../common.h"

using namespace std;

int main() {
    FileParser fp("2022/2-data-test");

    // Parse file

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