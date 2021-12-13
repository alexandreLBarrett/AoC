#include "../helpers.h"
#include "../common.h"

#include <numeric>
#include <queue>
#include <span>

using namespace std;

struct DeepSpot {
    size_t x = SIZE_MAX;
    size_t y = SIZE_MAX;
    size_t height = SIZE_MAX;

    bool operator<(const DeepSpot& other) const {
        return height < other.height;
    }

    bool operator==(const DeepSpot& other) const {
        return x == other.x && y == other.y;
    }
};

class SeaFloor {
private:
    vector<size_t> floorHeightmap;
    size_t height = 0;
    size_t width = SIZE_MAX;

    size_t getHeight(int x, int y) {
        int index = y * width + x;
        if (
            x >= width || x < 0 ||
            y >= height || y < 0
        ) return SIZE_MAX;

        return floorHeightmap[index];
    }

    bool checkCoord(int x, int y, size_t height) {
        return min({ 
            getHeight(x, y + 1),
            getHeight(x, y - 1),
            getHeight(x + 1, y),
            getHeight(x - 1, y),
        }) > height;
    }
public:
    SeaFloor(ifstream& stream) {
        for (std::istreambuf_iterator<char> it(stream), end; it != end; ++it) {
            if (*it == '\n') {
                width = min(floorHeightmap.size(), width);
                height++;
                continue;
            }
            floorHeightmap.push_back(*it - '0');   
        }
        height++;
    }

    vector<DeepSpot> getDeepSpots() {
        vector<DeepSpot> spots;

        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                auto spotHeight = getHeight(x, y);

                if (checkCoord(x, y, spotHeight))
                    spots.push_back(DeepSpot{x, y, spotHeight});
            }
        }

        return spots;
    }

    int getBasinSize(const DeepSpot& deepSpot) {
        unordered_map<DeepSpot, bool, decltype([](const DeepSpot& spot) -> size_t {
            return spot.x * 6 + spot.y * 8 + spot.height * 15;
        })> checked;
        checked[deepSpot] = true;

        queue<DeepSpot> nextSpots;
        nextSpots.push(deepSpot);

        int basinSize = 1;

        auto addIfOk = [this, &nextSpots, &checked, &basinSize](size_t x, size_t y, size_t comparedTo) {
            size_t height = getHeight(x, y);
            if (height == SIZE_MAX) return;

            if (height == 9) return;

            if (height < comparedTo) return;

            DeepSpot ds{x, y, height};

            if (!checked.contains(ds)) {
                nextSpots.push(ds);
                checked[ds] = true;
                basinSize++;
            }
        };

        while(nextSpots.size() > 0) {
            auto spot = nextSpots.front();
            nextSpots.pop();

            addIfOk(spot.x + 1, spot.y, spot.height);
            addIfOk(spot.x - 1, spot.y, spot.height);
            addIfOk(spot.x, spot.y + 1, spot.height);
            addIfOk(spot.x, spot.y - 1, spot.height);
        }

        return basinSize;
    }
};

int main() {
    FileParser fp("2021/9-data");
    auto seaFloor = fp.parseOne<SeaFloor>();

    DayPartHandler<ostream> dph;
    
    dph.AddPart([&](auto& out) mutable {
        size_t height = 0;
        auto deepSpots = seaFloor.getDeepSpots();

        for (auto spot : deepSpots) {
            height += spot.height + 1;
        }

        out = [=](auto& o) { o << height << endl; };
    });

    dph.AddPart([&](auto& out) mutable {
        size_t height = 0;
        auto deepSpots = seaFloor.getDeepSpots();

        vector<int> spotsSizes;
        for (auto spot : deepSpots) {
            spotsSizes.push_back(seaFloor.getBasinSize(spot));
        }

        sort(spotsSizes.begin(), spotsSizes.end(), greater<int>());

        span<int, 3> s{spotsSizes.begin(), 3};

        height = accumulate(s.begin(), s.end(), 1, [](auto a, auto b){ return a * b; });

        out = [=](auto& o) { o << height << endl; };
    });

    dph.RunAll(cout);
}