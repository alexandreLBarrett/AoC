#include "../helpers.h"
#include "../common.h"


#include <map>
#include <thread>
#include <semaphore>
#include <numeric>

using namespace std;

// puzzle: https://adventofcode.com/2023/day/8
struct Node {
    std::string name, left, right;
    Node* leftNode;
    Node* rightNode;

    friend istream& operator>>(istream& is, Node& node) {
        is >> node.name;
        is.ignore(" = ("sv.length());
        getline(is, node.left, ',');
        is.ignore(1);
        getline(is, node.right, ')');
        is.ignore(1);
        return is;
    }
};

int gcd(int a, int b)
{
    while (b)
    {
        int t = b;
        b = a % b;
        a = t;
    }

    return a;
}

int main() {
    FileParser fp("2023/8-data");

    // Parse file
    string path;
    fp.parseOne<void>([&](istream& is) {
        is >> path;
        is.ignore(2);
    });

    auto nodes = fp.parseRest<Node>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {

        // std::map<string, Node*> paths;
        // for (auto& node : nodes) {
        //     paths[node.name] = &node;
        // }

        // int i = 0;
        int steps = 0;
        // string current = "AAA";
        // while (current != "ZZZ") {
        //     auto node = paths[current];
        //     if (path[i] == 'L') {
        //         current = node->left;
        //     }
        //     else 
        //     {
        //         current = node->right;
        //     }

        //     i = (i + 1) % path.size();
        //     steps++;
        // }

        out = [=](auto& o) { o << steps << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        std::map<string, Node*> paths;
        
        std::vector<Node*> As;
        std::vector<Node*> Zs;
        for (auto& node : nodes) {
            paths[node.name] = &node;
            if (node.name[2] == 'A')
                As.push_back(&node);
            if (node.name[2] == 'Z')
                Zs.push_back(&node);
        }

        for (auto& node : nodes) {
            node.leftNode = paths[node.left];
            node.rightNode = paths[node.right];
        }

        int i = 0;

        auto pathSize = path.size();

        vector<int> zCycles;
        zCycles.resize(Zs.size());
        for (int j = 0; j < Zs.size(); ++j) {
            int i = 0;
            int count = 0;
            auto curr = Zs[j];
            string originalZ = curr->name;

            do {
                if (path[i] == 'L') {
                    curr = curr->leftNode;
                }
                else {
                    curr = curr->rightNode;
                }

                i = (i + 1) % pathSize;

                count++;
            } while (curr->name != originalZ);

            zCycles[j] = count;
        }

        sort(zCycles.begin(), zCycles.end());

        // Calculate LCM of all numbers (im lazy)
        // ...
        uint64_t lcm{};

        out = [=](auto& o) { o << lcm << endl; };
    });

    dph.RunAll(cout);
}