#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2023/day/6

int main() {
    FileParser fp("2023/6-data");

    // Parse file

    std::vector<int64_t> raceTimes;
    std::vector<int64_t> raceDistances;
    fp.parseOne<void>([&](istream& is) {
        string s;
        std::getline(is, s);
        stringstream ss{s};
        ss >> s; // skip Time:
        while (ss && !ss.eof()) {
            int64_t i;
            ss >> i;
            raceTimes.push_back(i);
        }

        std::getline(is, s);
        ss = stringstream{s};
        ss >> s; // skip Distance:
        while (ss && !ss.eof()) {
            int64_t i;
            ss >> i;
            raceDistances.push_back(i);
        }
    });

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        int64_t total = 1;
        for (int i = 0; i < raceTimes.size(); ++i) {
            int64_t maxTime = raceTimes[i];
            int64_t maxDistance = raceDistances[i];

            int64_t racePossibleTimes{};
            for (int64_t t = 0; t < maxTime; ++t) {
                int64_t remainingTime = maxTime - t; // s
                int64_t launchSpeed = t; // mm/s
                int64_t timeToReachEnd = launchSpeed * remainingTime - maxDistance;
                if (timeToReachEnd > 0)
                {
                    racePossibleTimes++;
                }
            }

            total *= racePossibleTimes;
        }

        out = [=](auto& o) { o << total << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        string raceTimeStr{};
        for (int64_t time : raceTimes)
            raceTimeStr += to_string(time);

        string distanceStr{};
        for (int64_t dist : raceDistances)
            distanceStr += to_string(dist);

        int64_t maxTime = stoll(raceTimeStr);
        int64_t maxDistance = stoll(distanceStr);

        int64_t racePossibleTimes{};
        for (int64_t t = 0; t < maxTime; ++t) {
            int64_t remainingTime = maxTime - t; // s
            int64_t launchSpeed = t; // mm/s
            int64_t timeToReachEnd = launchSpeed * remainingTime - maxDistance;
            if (timeToReachEnd > 0)
            {
                racePossibleTimes++;
            }
        }

        out = [=](auto& o) { o << racePossibleTimes << endl; };
    });

    dph.RunAll(cout);
}