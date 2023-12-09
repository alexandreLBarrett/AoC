#include "../helpers.h"
#include "../common.h"

#include <stack>

using namespace std;

// puzzle: https://adventofcode.com/2023/day/9

struct History {
    vector<int> history;

    friend istream& operator>>(istream& is, History& his) {
        string line;
        getline(is, line);
        stringstream lineSS{line};

        while(lineSS && !lineSS.eof()) {
            int val;
            lineSS >> val;
            his.history.push_back(val);
        }

        return is;
    }
};

int main() {
    FileParser fp("2023/9-data");

    // Parse file
    auto hists = fp.parseRest<History>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {

        int sum = 0;
        for (auto& hist : hists) {
            vector<int> currentHist = hist.history;

            stack<vector<int>> diffs;
            diffs.push(currentHist);

            while (!all_of(currentHist.begin(), currentHist.end(), [](auto elem) {
                return elem == 0;   
            })) {
                vector<int> currDiffs;
                for (int i = 0; i < currentHist.size() - 1; ++i)
                    currDiffs.push_back(currentHist[i + 1] - currentHist[i]);

                diffs.push(currDiffs);
                currentHist = diffs.top();
            }

            int acc = 0;
            diffs.pop();
            while (!diffs.empty()) 
            {
                auto& diff = diffs.top();

                acc += diff.back();

                diffs.pop();
            }
            sum += acc;
        }

        out = [=](auto& o) { o << sum << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        int sum = 0;
        for (auto& hist : hists) {
            vector<int> currentHist = hist.history;

            stack<vector<int>> diffs;
            diffs.push(currentHist);

            while (!all_of(currentHist.begin(), currentHist.end(), [](auto elem) {
                return elem == 0;   
            })) {
                vector<int> currDiffs;
                for (int i = 0; i < currentHist.size() - 1; ++i)
                    currDiffs.push_back(currentHist[i + 1] - currentHist[i]);

                diffs.push(currDiffs);
                currentHist = diffs.top();
            }

            int acc = 0;
            diffs.pop();
            while (!diffs.empty()) 
            {
                auto& diff = diffs.top();

                acc = diff.front() - acc;

                diffs.pop();
            }
            sum += acc;
        }

        out = [=](auto& o) { o << sum << endl; };
    });

    dph.RunAll(cout);
}