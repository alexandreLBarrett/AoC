#include "../helpers.h"
#include "../common.h"

using namespace std;

struct Target {
    int64_t x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    Target(ifstream& stream) {
        stream >> x1;
        stream >> x2;
        stream >> y1;
        stream >> y2;
    }
};

class World {
    Target target;

    bool isInTarget(const Point& p) {
        return 
            p.x >= target.x1 && p.x <= target.x2 && 
            p.y >= target.y1 && p.y <= target.y2;
    }

    bool isMissedTarget(const Point& p) {
        return p.y < target.y1;
    }

public:
    struct TrajectoryResult { 
        bool hit = false;
        int64_t maxHeight = 0;
        Point initialTrajectory = { 0, 0 };
        Point hitPoint = { 0, 0 };
    };  

    World(Target target) 
        : target(target)
    {}

    TrajectoryResult calculateTrajectory(int64_t velX, int64_t velY) {
        Point ballPos{ 0, 0 };
        Point baseVel = { velX, velY };
        int64_t maxHeight = ballPos.y;

        for (;;) {
            if (isInTarget(ballPos))
                return { true, maxHeight, baseVel, ballPos };

            if (isMissedTarget(ballPos))
                return { false };

            ballPos.x += velX;
            ballPos.y += velY;

            maxHeight = max(maxHeight, ballPos.y);

            velX = max((int64_t) 0, velX - 1);
            velY -= 1;
        }
    }
};

int main() {
    FileParser fp("2021/17-data");

    // Parse file
    auto target = fp.parseOne<Target>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        World world(target);

        World::TrajectoryResult bestResult;
        for (int x = 0; x < 500; x++) {
            for (int y = 0; y < 500; y++) {
                auto result = world.calculateTrajectory(x, y);

                if (result.maxHeight > bestResult.maxHeight) {
                    bestResult = result;
                }
            }
        }

        out = [=](auto& o) { o << bestResult.initialTrajectory.x << ", " << bestResult.initialTrajectory.y << " with max height of " << bestResult.maxHeight << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        World world(target);

        auto t1 = world.calculateTrajectory(6, 0);
        auto t2 = world.calculateTrajectory(7, -1);

        vector<World::TrajectoryResult> bestResults;
        for (int64_t x = 0; x < 500; x++) {
            for (int64_t y = -500; y < 500; y++) {
                auto result = world.calculateTrajectory(x, y);

                if (result.hit) {
                    bestResults.push_back(result);
                    // if (find_if(begin(bestResults), end(bestResults), [&result](const auto& item){ return item.initialTrajectory == result.initialTrajectory; }) == end(bestResults)) {
                    // }
                }
            }
        }

        out = [=](auto& o) { o << bestResults.size() << endl; };
    });

    dph.RunAll(cout);
}