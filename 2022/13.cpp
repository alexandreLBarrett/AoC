#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/13

class Node {
public:
    virtual unique_ptr<Node> clone() const = 0;
};

class ValueNode : public Node {
    uint32_t value;
public:
    ValueNode(uint32_t value) : value(value) {}

    unique_ptr<Node> clone() const {
        return make_unique<ValueNode>(value);
    }
};

class ContainerNode : public Node {
    vector<unique_ptr<Node>> children;
public:
    ContainerNode() = default;
    ContainerNode(const ContainerNode& node) {
        *this = node;
    }
    ContainerNode(istream& is) {
        char c;
        is >> c;
        while (c != ']') {
            if (c == ',')
                is >> c;

            if (c == '[') {
                children.push_back(make_unique<ContainerNode>(is));
                is >> c;
            } else {
                string currentVal = "";
                while (c != ',' && c != ']') {
                    currentVal += c;
                    is >> c;
                }
                children.push_back(make_unique<ValueNode>(stoi(currentVal)));
            }
        }
    }

    ContainerNode& operator=(const ContainerNode& other) {
        for (int i = 0; i < other.children.size(); ++i) {
            children.push_back(other.children[i]->clone());
        }
        return *this;
    }

    unique_ptr<Node> clone() const {
        unique_ptr<ContainerNode> node = make_unique<ContainerNode>();
        for (int i = 0; i < children.size(); ++i) {
            node->children.push_back(children[i]->clone());
        }
        return node;
    }
};

class Comparison {
    unique_ptr<ContainerNode> left;
    unique_ptr<ContainerNode> right;
public:
    Comparison(istream& is) {
        char c;
        is >> c; // burn [
        left = make_unique<ContainerNode>(is);
        is >> c; // burn [
        right = make_unique<ContainerNode>(is);
        string s;
        getline(is, s);
    }

    bool inOrder() {
        return false;
    }
};

int main() {
    FileParser fp("2022/13-data-test");

    // Parse file
    auto comps = fp.parseRest<Comparison>();

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