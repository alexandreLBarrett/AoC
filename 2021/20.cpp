#include "../helpers.h"
#include "../common.h"
#include "../grid.h"

#include <array>

using namespace std;

const auto toChar = [](const auto& value) { return value ? '#' : '.'; };

struct IEA {
    array<bool, 512> data;

    IEA(ifstream& stream) {
        uint32_t index = 0;

        string line;
        getline(stream, line);

        stringstream ss(line);
        for (istreambuf_iterator<char> it(ss), end; it != end; ++it) {
            data[index] = *it == '#';
            index++;
        }
        stream.ignore(1);
    }
};

class World {
public:
    Grid<bool> grid;
    World(ifstream& stream) {
        int x, y;
        x = y = 0;

        for (istreambuf_iterator<char> it(stream), end; it != end; ++it) {
            if (*it == '\n') {
                x = 0;
                y++;
            } else {
                grid.set(x, y, *it == '#');
                x++;
            }
        }
    }

    uint32_t enhancementOffset(GridNode<bool>* node) {
        uint32_t offset = 0;

        using Sibs = GridNode<bool>::Sibling;
        if (auto n = node->get(Sibs::TOP_LEFT); n)  offset |= (n->value ? 1 : 0) << 8;
        if (auto n = node->get(Sibs::TOP); n)       offset |= (n->value ? 1 : 0) << 7;
        if (auto n = node->get(Sibs::TOP_RIGHT); n) offset |= (n->value ? 1 : 0) << 6;
        if (auto n = node->get(Sibs::LEFT); n)      offset |= (n->value ? 1 : 0) << 5;

        offset |= (node->value ? 1 : 0) << 4;

        if (auto n = node->get(Sibs::RIGHT); n)        offset |= (n->value ? 1 : 0) << 3;
        if (auto n = node->get(Sibs::BOTTOM_LEFT); n)  offset |= (n->value ? 1 : 0) << 2;
        if (auto n = node->get(Sibs::BOTTOM); n)       offset |= (n->value ? 1 : 0) << 1;
        if (auto n = node->get(Sibs::BOTTOM_RIGHT); n) offset |= (n->value ? 1 : 0) << 0;

        return offset;
    }

    void enhance(const IEA& iea) {
        Grid<bool> copy = grid;

        grid.foreachPixel([&](auto node) {
            auto id = enhancementOffset(node);
            auto value = iea.data[id];
            copy.set(node->pos.x, node->pos.y, value);
        });

        grid = copy;
    }

    uint64_t countDots() const {
        uint64_t total = 0;
        grid.foreachNode([&](auto node) {
            if (node->value)
                total++;
        });
        return total;
    }

    friend ostream& operator<<(ostream& os, const World& world) {
        world.grid.print(os, toChar);
        return os;
    }
};

int main() {
    FileParser fp("2021/20-data-test");

    // Parse file
    auto iea = fp.parseOne<IEA>();
    auto world = fp.parseOne<World>();

    // Grid<bool> g;

    // g.set(-1, 0, true);
    // g.set(-2, 0, true);
    // g.set(-3, 0, true);
    // g.print(cout, toChar);

    // world.grid = g;
    // world.enhance(iea);
    // world.grid.print(cout, toChar);

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        for (int i = 0; i < 2; i++) {
            world.enhance(iea);
        }
        out = [=](auto& o) {
            o << world.countDots() << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}