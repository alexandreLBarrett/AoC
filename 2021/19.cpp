#include "../helpers.h"
#include "../common.h"

using namespace std;

struct Report {
    int64_t x = 0, y = 0, z = 0;
};

class Scanner {
    uint64_t index;
    vector<Report> reports;
public:
    Scanner(ifstream& stream) {
        string token;
        stream >> token; // skips ---
        stream >> token; // skips scanner
        stream >> token; // gets good value

        index = atoi(token.c_str());

        getline(stream, token); // clears rest of line

        do {
            

            Report rep;

            getline(stream, token, ',');
            rep.x = atoi(token.c_str());
            getline(stream, token, ',');
            rep.y = atoi(token.c_str());
            getline(stream, token);
            rep.z = atoi(token.c_str());

            reports.push_back(rep);

            auto restore = stream.tellg();
            getline(stream, token);
            stream.seekg(restore, ios_base::beg);
        } while (token != "");
        getline(stream, token);
    }
};

int main() {
    FileParser fp("2021/19-data-test");

    // Parse file
    auto scanners = fp.parseRest<Scanner>();

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